#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetColor(39);
	ofNoFill();
	ofSetLineWidth(2);

	this->font_size = 130;
	this->font.loadFont("fonts/Kazesawa-bold.ttf", this->font_size, true, true, true);

	this->charactors = { 'R', 'E', 'L', 'A', 'Y' };

	this->index = 0;
	this->progress = 0;
}

//--------------------------------------------------------------
void ofApp::update() {

	if (ofGetFrameNum() % 60 == 0) {

		this->index = (this->index + 1) % this->charactors.size();
		this->progress = 0;
	}
	this->progress += this->progress < 100 ? 2 : 0;
}

//--------------------------------------------------------------
void ofApp::draw() {

	int tmp_index = this->index;
	for (int i = 0; i < this->charactors.size(); i++) {

		int next_index = (tmp_index + 1) % this->charactors.size();
		this->draw_blend_charactor(ofPoint(144 * i + 72, ofGetHeight() * 0.5), this->charactors[tmp_index], this->charactors[next_index], this->progress);
		tmp_index = next_index;
	}
}

//--------------------------------------------------------------
void ofApp::draw_blend_charactor(ofPoint point, char c1, char c2, float progress) {

	int total_sample_count = 100;

	ofPushMatrix();
	ofTranslate(point);

	ofPath path = this->font.getCharacterAsPoints(c1, true, false);
	vector<ofPolyline> outline = path.getOutline();
	int sample_count = total_sample_count / outline.size();
	vector<ofPoint> points;
	for (int outline_index = 0; outline_index < (int)outline.size(); outline_index++) {

		outline[outline_index] = outline[outline_index].getResampledByCount(sample_count);
		vector<glm::vec3> vertices = outline[outline_index].getVertices();
		for (size_t vertices_index = 0; vertices_index < vertices.size(); vertices_index++) {

			ofPoint point(vertices[vertices_index].x - this->font_size * 0.5, vertices[vertices_index].y + this->font_size * 0.5, vertices[vertices_index].z);
			points.push_back(point);
		}
	}

	ofPath next_path = this->font.getCharacterAsPoints(c2, true, false);
	vector<ofPolyline> next_outline = next_path.getOutline();
	int next_sample_count = total_sample_count / next_outline.size();
	vector<ofPoint> next_points;
	for (int outline_index = 0; outline_index < (int)next_outline.size(); outline_index++) {

		next_outline[outline_index] = next_outline[outline_index].getResampledByCount(next_sample_count);
		vector<glm::vec3> vertices = next_outline[outline_index].getVertices();
		for (size_t vertices_index = 0; vertices_index < vertices.size(); vertices_index++) {

			ofPoint point(vertices[vertices_index].x - this->font_size * 0.5, vertices[vertices_index].y + this->font_size * 0.5, vertices[vertices_index].z);
			next_points.push_back(point);
		}
	}

	vector<ofPoint> draw_points;
	for (int i = 0; i < total_sample_count; i++) {

		if (i < points.size() && i < next_points.size()) {

			ofPoint p = (next_points[i] - points[i]) * progress * 0.01 + points[i];
			draw_points.push_back(p);
		}
	}

	for (int i = 0; i < draw_points.size(); i++) {

		for (int j = i + 1; j < draw_points.size(); j++) {

			if (draw_points[i].distance(draw_points[j]) < 15) {

				ofDrawLine(draw_points[i], draw_points[j]);
			}
		}
	}

	ofPopMatrix();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
