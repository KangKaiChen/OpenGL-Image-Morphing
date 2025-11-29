
#include <FL/Fl.H>
#include <FL/Fl_Image.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Repeat_Button.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Box.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>
#include <fstream>
#include <iterator>
#include "Fl_ViewerCV.h"

using namespace std;
using namespace cv;

Fl_ViewerCV* test1;
Fl_ViewerCV* test2;
Fl_ViewerCV* result;
Fl_Widget* button_obj;
Fl_Slider* image_slider;

Mat obama, dilma,pikes;
static int height = 250, wedight = 200, win_height = 960, win_wedight = 550, frame = 7, position_flag = 0;
bool Apoint_flag = false, Mpoint_flag = false, Rpoint_flag = false, drag_point_flag = false;
vector<Point2f> morph_p1;
vector<Point2f> morph_p2;
vector<Mat> image_all;
OPENFILENAME file;          // global

struct correspondens {
	std::vector<int> index;
};

bool Check_PointandMouse(Point now, Point former, int distance)
{
	if (abs(now.x - former.x) <= distance && abs(now.y - former.y) <= distance)
		return true;
	return false;
}
int Fl_ViewerCV::handle(int event) {

	if (Fl_Widget::active())
	{
		//Mouse events are passed to opencv if an OpenCV callback function has been passed
		if (mouse_cb != NULL)
		{
			if (event == FL_PUSH || event == FL_DRAG || event == FL_MOVE || event == FL_RELEASE) {
				if (mouse_cb && image && (Fl::event_x() - x()) < image->cols && (Fl::event_y() - y()) < image->rows)
					mouse_cb(fl_event2cv_event(event), Fl::event_x() - x(), Fl::event_y() - y(), 0, CVcb_param);
			}
		}
		Point2f clk_p, mv_p;
		static int move_position = 0;
		Mat image1, image2;
		obama.copyTo(image1);
		dilma.copyTo(image2);
		switch (event)
		{
		case FL_PUSH://----------如果按下滑鼠
			if (Fl::event_x() <= 370) {
				clk_p.x = Fl::event_x() - 170.0f;
				clk_p.y = Fl::event_y() - 10.0f;
				position_flag = 1;
				printf("image1:%f ", clk_p.x);
				printf("%f\n", clk_p.y);
				if (Apoint_flag) {
					morph_p1.push_back(clk_p);
					printf("image1 already have:%f\n", morph_p1.size());
					for (auto& i : morph_p1)
						circle(image1, i, 3, Scalar(255, 20, 147), -1);
					test1->SetImage(&image1);
					test1->UpdateView();
					break;
				}
				else {
					if (morph_p1.size() != 0)
					{
						bool first = true;
						for (int i = 0; i < morph_p1.size(); i++)
						{
							if (first)
								drag_point_flag = false;
							if (first && Check_PointandMouse(clk_p, morph_p1[i], 5))
							{
								if (Rpoint_flag) {
									cout << "remove image1 point: " << morph_p1[i] << endl;
									morph_p1.erase(morph_p1.begin() + i);
									for (auto& i : morph_p1)
										circle(image1, i, 3, Scalar(255, 20, 147), -1);
									test1->SetImage(&image1);
									test1->UpdateView();
									continue;
								}
								mv_p = morph_p1[i];
								move_position = i;
								first = false;
								drag_point_flag = true;
								continue;
							}
						}
					}
				}
			}
			else {
				clk_p.x = Fl::event_x() - 730.0f;
				clk_p.y = Fl::event_y() - 10.0f;
				position_flag = 2;
				printf("image2:%f ", clk_p.x);
				printf("%f\n", clk_p.y);
				if (Apoint_flag) {
					morph_p2.push_back(clk_p);
					printf("image2 already have:%f\n", morph_p2.size());
					for (auto& i : morph_p2)
						circle(image2, i, 3, Scalar(255, 0, 0), -1);
					test2->SetImage(&image2);
					test2->UpdateView();
					break;
				}
				else {
					if (morph_p2.size() != 0)
					{
						bool first = true;
						for (int i = 0; i < morph_p2.size(); i++)
						{
							if (first)
								drag_point_flag = false;
							if (first && Check_PointandMouse(clk_p, morph_p2[i], 5))
							{
								if (Rpoint_flag) {
									cout << "remove image2 point: " << morph_p2[i] << endl;
									morph_p2.erase(morph_p2.begin() + i);
									for (auto& i : morph_p2)
										circle(image2, i, 3, Scalar(255, 0, 0), -1);
									test2->SetImage(&image2);
									test2->UpdateView();
									continue;
								}
								mv_p = morph_p2[i];
								move_position = i;
								first = false;
								drag_point_flag = true;
								continue;
							}
						}
					}
				}
			}
			return 1;
		case FL_DRAG:
			if (Mpoint_flag)
			{
				if (position_flag == 1 && drag_point_flag) {
					printf("image1.x:%d,image1.y:%d\n", Fl::event_x(), Fl::event_y());
					morph_p1[move_position].x = Fl::event_x() - 170.0f;
					morph_p1[move_position].y = Fl::event_y() - 10.0f;
					for (auto& i : morph_p1)
						circle(image1, i, 3, Scalar(0, 0, 255), -1);
					test1->SetImage(&image1);
					test1->UpdateView();
				}
				else if (position_flag == 2 && drag_point_flag) {
					morph_p2[move_position].x = Fl::event_x() - 730.0f;
					morph_p2[move_position].y = Fl::event_y() - 10.0f;
					for (auto& i : morph_p2)
						circle(image2, i, 3, Scalar(0, 0, 255), -1);
					test2->SetImage(&image2);
					test2->UpdateView();
				}
			}
			break;
		}
	}
	return(Fl_Widget::handle(event));
}

Subdiv2D creat_subdiv(Mat& img, vector<Point2f>& points) {
	Size size = img.size();
	Rect rect(0, 0, size.width, size.height);
	Subdiv2D subdiv(rect);
	subdiv.insert(points);
	return subdiv;
}
static void draw_delaunay(Mat& img, Subdiv2D& subdiv, const Scalar& delaunay_color) {
	vector<Vec6f> triangleList;
	subdiv.getTriangleList(triangleList);
	vector<Point> pt(3);
	Size size = img.size();
	Rect rect(0, 0, size.width, size.height);
	for (auto t : triangleList) {
		pt[0] = Point(cvRound(t[0]), cvRound(t[1]));
		pt[1] = Point(cvRound(t[2]), cvRound(t[3]));
		pt[2] = Point(cvRound(t[4]), cvRound(t[5]));
		// Draw rectangles completely inside the image.
		if (rect.contains(pt[0]) && rect.contains(pt[1]) && rect.contains(pt[2])) {
			line(img, pt[0], pt[1], delaunay_color, 1, LINE_AA, 0);
			line(img, pt[1], pt[2], delaunay_color, 1, LINE_AA, 0);
			line(img, pt[2], pt[0], delaunay_color, 1, LINE_AA, 0);
		}
	}
}
void AffineTransform(Mat& warpImage, Mat& src, vector<Point2f>& srcTri, vector<Point2f>& dstTri)
{
	Mat warpMat = getAffineTransform(srcTri, dstTri);

	warpAffine(src, warpImage, warpMat, warpImage.size(), INTER_LINEAR, BORDER_REFLECT_101);
}
void morphing(Mat& img1, Mat& img2, Mat& img, vector<Point2f>& t1, vector<Point2f>& t2, vector<Point2f>& t, double alpha)
{
	Rect r = boundingRect(t);
	Rect r1 = boundingRect(t1);
	Rect r2 = boundingRect(t2);

	vector<Point2f> t1Rect, t2Rect, tRect;
	vector<Point> tRectInt;
	for (int i = 0; i < 3; i++)
	{
		tRect.push_back(Point2f(t[i].x - r.x, t[i].y - r.y));
		tRectInt.push_back(Point(t[i].x - r.x, t[i].y - r.y));

		t1Rect.push_back(Point2f(t1[i].x - r1.x, t1[i].y - r1.y));
		t2Rect.push_back(Point2f(t2[i].x - r2.x, t2[i].y - r2.y));
	}
	Mat mask = Mat::zeros(r.height, r.width, CV_32FC3);
	fillConvexPoly(mask, tRectInt, Scalar(1.0, 1.0, 1.0), 16, 0);

	Mat img1Rect, img2Rect;
	img1(r1).copyTo(img1Rect);
	img2(r2).copyTo(img2Rect);

	Mat warpImage1 = Mat::zeros(r.height, r.width, img1Rect.type());
	Mat warpImage2 = Mat::zeros(r.height, r.width, img2Rect.type());

	AffineTransform(warpImage1, img1Rect, t1Rect, tRect);
	AffineTransform(warpImage2, img2Rect, t2Rect, tRect);

	Mat imgRect = (1.0 - alpha) * warpImage1 + alpha * warpImage2;
	imgRect.convertTo(imgRect, CV_32FC3);
	multiply(imgRect, mask, imgRect);
	multiply(img(r), Scalar(1.0, 1.0, 1.0) - mask, img(r));
	img(r) = img(r) + imgRect;

}

void quit(Fl_Widget*, void*)//關閉視窗
{
	exit(0);
}
void reset(Fl_Widget*, void*)
{
	Apoint_flag = FALSE;
	Mpoint_flag = FALSE;
	Rpoint_flag = FALSE;
	if (!morph_p1.empty())
		morph_p1.clear();
	if (!morph_p2.empty())
		morph_p2.clear();
	if (!image_all.empty())
		image_all.clear();
	test1->SetImage(&obama);
	test1->UpdateView();
	test2->SetImage(&dilma);
	test2->UpdateView();
	result->SetImage(&pikes);
	result->UpdateView();
}
void morph(Fl_Widget*, void*)
{
	if (morph_p1.empty() || morph_p2.empty()) {
		printf("ERROR: feature point is empty, stop morphing!!!!\n");
		return;
	}
	if (morph_p1.size() != morph_p2.size()) {
		printf("ERROR: feature point number isn't align , stop morphing!!!!\n");
		return;
	}
	if (!image_all.empty())
		image_all.clear();
	Mat img1, img2, image1, image2;
	vector<Vec6f> trangleL1;
	obama.copyTo(img1);
	dilma.copyTo(img2);
	Mat img1_copy, img2_copy;
	img1.copyTo(img1_copy);
	img2.copyTo(img2_copy);
	Size size1 = img1.size();
	Rect rect1(0, 0, size1.width, size1.height);
	auto subdiv1 = creat_subdiv(img1, morph_p1);
	auto subdiv2 = creat_subdiv(img2, morph_p2);
	Scalar color(255, 255, 255);
	draw_delaunay(img1_copy, subdiv1, color);
	draw_delaunay(img2_copy, subdiv2, color);
	test1->SetImage(&img1_copy);
	test1->UpdateView();
	test2->SetImage(&img2_copy);
	test2->UpdateView();

	subdiv1.insert(morph_p1);
	subdiv1.getTriangleList(trangleL1);
	auto len = trangleL1.size();

	obama.convertTo(image1, CV_32F);
	dilma.convertTo(image2, CV_32F);

	vector<vector<int>> index;
	Size size = img1.size();
	for (int i = 0; i < len; ++i) {
		vector<pair<Point2f, int>> cur;
		for (int j = 0; j <= 4; j += 2) {
			cur.emplace_back(Point2f(trangleL1[i][j], trangleL1[i][j + 1]), 0);
			for (auto k = 0; k < morph_p1.size(); ++k) {
				if (morph_p1[k] == cur.back().first) {
					cur[cur.size() - 1].second = k;
					break;
				}
			}
		}
		Rect rect(0, 0, size.width, size.height);
		if (rect.contains(cur[0].first) && rect.contains(cur[1].first) && rect.contains(cur[2].first)) {
			index.emplace_back(vector<int>({ cur[0].second, cur[1].second, cur[2].second }));
		}
	}
	cout << frame << endl;
	for (int id = 0; id < frame; ++id) {
		auto alpha = static_cast<float>(1. * id / (frame - 1));
		Mat img_res = Mat::zeros(img1.size(), CV_32FC3);
		vector<Point2f> key_points_res;
		for (auto i = 0; i < morph_p1.size(); ++i) {
			auto x = (1. - alpha) * morph_p1[i].x + alpha * morph_p2[i].x;
			auto y = (1. - alpha) * morph_p1[i].y + alpha * morph_p2[i].y;
			key_points_res.emplace_back(x, y);
		}
		for (const auto& i : index) {
			vector<Point2f> t1, t2, t_res;
			for (auto j = 0; j < 3; ++j) {
				t1.push_back(morph_p1[i[j]]);
				t2.push_back(morph_p2[i[j]]);
				t_res.push_back(key_points_res[i[j]]);
			}
			morphing(img1, img2, img_res, t1, t2, t_res, alpha);
		}
		img_res.convertTo(img_res, CV_8UC3);
		image_all.push_back(img_res);
		result->SetImage(&img_res);
		result->UpdateView();
		printf("image done\n");
	}
	image_slider->value(frame);
}

void save_point(Fl_Widget*, void*)
{
	if (morph_p1.empty() || morph_p2.empty()) {
		cout << "ERROR: point can't be empty!!!!" << endl;
		return;
	}
	ofstream output_file("point_list1.txt");
	if (!output_file)cout << "ERROR" << endl;
	for (const auto& i : morph_p1) output_file << i.x << " " << i.y << endl;
	cout << "point already SAVE to point_list1.txt file !!!!" << endl;
	ofstream output_file2("point_list2.txt");
	if (!output_file2)cout << "ERROR" << endl;
	for (const auto& i : morph_p2) output_file2 << i.x << " " << i.y << endl;
	cout << "point already SAVE to point_list2.txt file !!!!" << endl;
}
void remove_point(Fl_Widget*, void*)
{
	Rpoint_flag = TRUE;
	Mpoint_flag = FALSE;
	Apoint_flag = FALSE;
}
void move_point(Fl_Widget*, void*)
{
	Mpoint_flag = TRUE;
	Apoint_flag = FALSE;
	Rpoint_flag = FALSE;
}
void copy_point(Fl_Widget*, void*)
{
	Mat image1, image2;
	obama.copyTo(image1);
	dilma.copyTo(image2);
	if (!morph_p1.empty()) {
		morph_p2 = morph_p1;
		for (auto& i : morph_p2)
			circle(image2, i, 1, (0, 0, 255), 4);
		test2->SetImage(&image2);
		test2->UpdateView();
		cout << "copy completed!!!" << endl;
	}
	else cout << "ERROR" << endl;
}
void add_point(Fl_Widget*, void*)
{
	Apoint_flag = TRUE;
	Mpoint_flag = FALSE;
	Rpoint_flag = FALSE;
}
void load_point(Fl_Widget*, void*)
{
	if (!morph_p1.empty())
		morph_p1.clear();
	if (!morph_p2.empty())
		morph_p2.clear();
	Mat image1, image2;
	obama.copyTo(image1);
	dilma.copyTo(image2);
	ifstream ifs("point_list1.txt");
	if (!ifs) {
		printf("Error\n");
		return;
	}
	float x, y;
	while (ifs >> x >> y)
	{
		morph_p1.push_back(Point2f(x, y));
	}
	for (const auto& i : morph_p1) circle(image1, i, 1, (0, 0, 255), 4);//cout << i << endl;
	test1->SetImage(&image1);
	test1->UpdateView();
	printf("point already READ to point_list1.txt file\n");

	ifstream ifs2("point_list2.txt");
	if (!ifs2) {
		printf("Error");
		return;
	}
	while (ifs2 >> x >> y)
	{
		morph_p2.push_back(Point2f(x, y));
	}
	for (const auto& i : morph_p2) circle(image2, i, 1, (0, 0, 255), 4);//cout << i << endl;
	test2->SetImage(&image2);
	test2->UpdateView();
	printf("point alreafy READ to point_list2.txt file\n");
}
void load_image(Fl_Widget*, void*)
{
	if (!morph_p1.empty())
		morph_p1.clear();
	if (!morph_p2.empty())
		morph_p2.clear();
	if (file.lpstrFile) { delete[] file.lpstrFile; }
	if (file.lpstrInitialDir) { delete[] file.lpstrInitialDir; }
	memset((void*)&file, 0, sizeof(file));
	file.lStructSize = sizeof(OPENFILENAME);
	file.Flags |= OFN_NOVALIDATE;          // prevent disabling of front slashes
	file.Flags |= OFN_HIDEREADONLY;        // hide readonly flag
	file.Flags |= OFN_EXPLORER;            // use 'newer' explorer windows
	file.Flags |= OFN_ENABLESIZING;        // allow window to be resized
	file.Flags |= OFN_NOCHANGEDIR;         // prevent dialog for messing up the cwd
	file.nMaxFile = 4096 - 1;
	file.lpstrFile = new char[4096];
	file.lpstrFile[0] = 0;
	file.hwndOwner = GetForegroundWindow();
	file.lpstrTitle = "Open some file";
	int err = GetOpenFileName(&file);
	if (err == 0) {
		err = CommDlgExtendedError();           // extended error check
		if (err == 0) return;                 // user hit 'cancel'
		fprintf(stderr, "CommDlgExtendedError() code=%d", err);
		return;
	}
	printf("User picked '%s'\n", file.lpstrFile);
	cout << "change image to: " << position_flag << endl;
	Mat img = imread(file.lpstrFile);
	resize(img, img, Size(200, 252), 0, 0, INTER_LINEAR);
	if (position_flag == 1) {
		obama = img;
		test1->SetImage(&img);
		test1->UpdateView();
	}
	else if (position_flag == 2) {
		dilma = img;
		test2->SetImage(&img);
		test2->UpdateView();
	}

}
void slider(Fl_Widget*, void*) {
	//cout << alpha_slider->value() << endl;
	image_slider->bounds(1, frame);
	if (!image_all.empty()) {
		result->SetImage(&image_all[(int)image_slider->value() - 1]);
		result->UpdateView();
	}
}

int main(int argc, char** argv)
{

	Fl_Window window(win_height, win_wedight);// Create a FLTK window. Resolution 1050*400.

	pikes = imread("berkeley-campanile.bmp");

	obama = imread("obama.bmp");
	test1 = new Fl_ViewerCV(170, 10, wedight, height);
	test1->SetImage(&obama);
	test1->UpdateView();

	dilma = imread("dilma.bmp");
	test2 = new Fl_ViewerCV(170 + (wedight + 80) * 2, 10, wedight, height);
	test2->SetImage(&dilma);
	test2->UpdateView();

	result = new Fl_ViewerCV(170 + (wedight + 80), 250, wedight-20, height-10);
	result->SetImage(&pikes);
	result->UpdateView();

	button_obj = new Fl_Button(10, 380, 130, 30, "Quit");
	button_obj->callback(quit);
	button_obj = new Fl_Button(10, 340, 130, 30, "Reset");
	button_obj->callback(reset);
	button_obj = new Fl_Button(10, 300, 130, 30, "Morph");
	button_obj->callback(morph);
	button_obj = new Fl_Button(10, 260, 130, 30, "Save Point");
	button_obj->callback(save_point);
	button_obj = new Fl_Button(10, 220, 130, 30, "Remove Point");
	button_obj->callback(remove_point);
	button_obj = new Fl_Button(10, 180, 130, 30, "Move Point");
	button_obj->callback(move_point);
	button_obj = new Fl_Button(10, 140, 130, 30, "Copy Point");
	button_obj->callback(copy_point);
	button_obj = new Fl_Button(10, 100, 130, 30, "Add Point");
	button_obj->callback(add_point);
	button_obj = new Fl_Button(10, 60, 130, 30, "Load Point");
	button_obj->callback(load_point);
	button_obj = new Fl_Button(10, 20, 130, 30, "Load Image");
	button_obj->callback(load_image);
	image_slider = new Fl_Value_Slider(415, win_wedight - 60, 250, 30);
	image_slider->type(FL_HOR_SLIDER);
	image_slider->bounds(1, frame);
	image_slider->value(frame);
	image_slider->callback(slider);
	window.end();			 // End of FLTK windows setting.
	window.show(argc, argv); // Show the FLTK window
	return Fl::run();
}