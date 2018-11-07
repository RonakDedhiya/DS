
#ifdef USE_FACE_NET
#include "FaceNetFeatureGetter.cpp"

#else
	#ifdef USE_MOBILE_NET
		#include "MobileNetFeatureGetter.cpp"
	#else
		#ifdef USE_CAFFE_SHUFFE_NET
			#include "CaffeShuffeNetFeatureGetter.cpp"
		#else

#include "FeatureGetter.h"


#include <tensorflow/core/public/session.h>
#include <fstream>
#include <iostream>

#include <tensorflow/cc/saved_model/loader.h>
#include <tensorflow/core/graph/default_device.h>
#include <tensorflow/core/platform/env.h>
#include <tensorflow/core/protobuf/config.pb.h>
#include <tensorflow/c/checkpoint_reader.h>
#include <tensorflow/c/c_api_internal.h>
#include <opencv2/opencv.hpp>
#include <tensorflow/cc/ops/math_ops.h>
namespace tf = tensorflow;

#include <sys/time.h>


static int64_t fgtm() {
	struct timeval tm;
	gettimeofday(&tm, 0);
	int64_t re = ((int64_t)tm.tv_sec) * 1000 * 1000 + tm.tv_usec;
	return re;
}


boost::shared_ptr<FeatureGetter> FeatureGetter::self_;

typedef unsigned char uint8;

std::unique_ptr<tf::Session> session;

void tobuffer(const cv::Mat &img, uint8 *buf) {
	int pos = 0;
//	for (cv::Mat img : imgs) {
		int LLL = img.cols*img.rows * 3;
		int nr = img.rows;
		int nc = img.cols;
		if (img.isContinuous())
		{
			nr = 1;
			nc = LLL;
		}

		for (int i = 0; i < nr; i++)
		{
			const uchar* inData = img.ptr<uchar>(i);
			for (int j = 0; j < nc; j++)
			{
				buf[pos] = *inData++;
				pos++;
			}
		}
//	}
}



typedef std::vector<double> DSR;
typedef std::vector<DSR> DSRS;
typedef std::vector<int> IDSR;
typedef std::vector<IDSR> IDSRS;


	bool FeatureGetter::Init() {
        tf::Session* session_ptr;
        auto status = NewSession(tf::SessionOptions(), &session_ptr);
        if (!status.ok()) {
            std::cout << status.ToString() << "\n";
            return false;
        }
        session.reset(session_ptr);

        //------------------
        tf::GraphDef graph_def;

        auto status1 = ReadBinaryProto(tf::Env::Default(), "./data/mars-small128-1.4.pb", &graph_def);
        if (!status1.ok()) {
            printf("ReadBinaryProto failed: %s\n", status1.ToString().c_str());
			return false;
        }

        status = session->Create(graph_def);
        if (!status.ok()) {
            printf("create graph in session failed: %s\n", status.ToString().c_str());
			return false;
        }

        std::vector<std::string> node_names;
        for (const auto &node : graph_def.node()) {
						//printf("node name:%s\n", node.name().c_str());
            node_names.push_back(node.name());
        }

		return true;
	}
	bool FeatureGetter::Get(const cv::Mat &img, const std::vector<cv::Rect> &rcs,
		std::vector<FFEATURE> &fts) {
        std::vector<cv::Mat> mats;
        for(cv::Rect rc:rcs){

						float new_width = 0.5 * rc.height;
						rc.x = rc.x -( (new_width - rc.width) / 2 );
				    rc.width = new_width;

						//pre-Processing
						rc.width = rc.x +rc.width;
						rc.height = rc.y + rc.height;

						rc.x = std::max(0,rc.x);
						rc.y = std::max(0,rc.y);
						rc.width = std::min(img.cols,rc.width);
						rc.height = std::min(img.rows,rc.height);

						rc.width = rc.width - rc.x;
						rc.height = rc.height - rc.y;
            cv::Mat mat1 = img(rc).clone();
            cv::resize(mat1, mat1, cv::Size(64, 128));
						// cv::imshow("mat1",mat1);
						// cv::waitKey(1);
            mats.push_back(mat1);
        }
        int count = mats.size();

				for (size_t i = 0; i < count; i++) {
					tensorflow::Tensor input_tensor0(tensorflow::DT_UINT8, { 1, 128, 64, 3 });
					tobuffer(mats[i], input_tensor0.flat<uint8>().data());

					std::vector<tensorflow::Tensor> output_tensor;
					std::vector<std::pair<std::string, tensorflow::Tensor>> ins;
					std::pair<std::string, tensorflow::Tensor> pa;
					pa.first = "images";
					pa.second = input_tensor0;
					ins.push_back(pa);
					std::vector<std::string> outnames;
					outnames.push_back("features");
					std::vector<std::string> ts;
					int64_t ftm1 = fgtm();
					// printf("tensor shape",input_tensor0.dims());
					auto status = session->Run(
						ins,
						outnames,
						ts,
						&output_tensor);
						if (!status.ok()) {
							printf("error 3%s \n", status.ToString().c_str());
							return false;
						}
						float *tensor_buffer = output_tensor[0].flat<float>().data();
						FFEATURE ft;
						// std::cout << "begin ---->" << '\n';
			      for (int j = 0; j < 128; j++) {
							ft(j) = tensor_buffer[j];
			        // printf(",%f", tensor_buffer[j]);
			      }
						// std::cout << "End --->" << '\n';
						fts.push_back(ft);

				}

		return true;
	}
		#endif
	#endif
#endif
