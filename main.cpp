#include <cstdlib>
#include <opencv2/opencv.hpp>

#define CELL_WIDTH 48
#define CELL_HEIGHT 75
#define TOLERANCE 20

int
main(int argc, char *argv[])
{
	const char *infile_name = "../passport_mrz_detector_cpp/ocrb.png";
	const char *outfile_names = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789<";
	cv::Mat in = cv::imread(infile_name, CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat work = in.clone();
	std::vector<std::vector <cv::Point> > contours;
	cv::findContours(work, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	std::vector<cv::Rect> brs;
	std::for_each(contours.begin(), contours.end(), [&brs](std::vector<cv::Point> contour) {
		cv::Rect br = cv::boundingRect(contour);
		if (
			br.width >= CELL_WIDTH - TOLERANCE
			&& br.width <= CELL_WIDTH + TOLERANCE
			&& br.height >= CELL_HEIGHT - TOLERANCE
			&& br.height <= CELL_HEIGHT + TOLERANCE
		) {
			brs.push_back(br);
		} else {
			std::cerr << "Rejecting contour with bbox: " << br << std::endl;
		}
	});
	std::sort(brs.begin(), brs.end(), [](cv::Rect &first, cv::Rect &second) {
		return first.x < second.x;
	});
	size_t i = 0;
	std::for_each(
		brs.begin(), brs.end(),
		[in, outfile_names, &i]
		(cv::Rect &br)
		{
			std::cerr << br << std::endl;
			cv::Mat ch = in(br);
			std::string outfile_name("out/");
			outfile_name += outfile_names[i];
			outfile_name += ".png";
			cv::imwrite(outfile_name, ch);
			i++;
		}
	);
	return EXIT_SUCCESS;
}
