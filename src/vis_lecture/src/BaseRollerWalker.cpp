#include <cmath>
#include <iostream>
#include "vis_lecture/BaseRollerWalker.hpp"


#define PI 3.1415926535897932384626433832795

BaseRollerWalker::BaseRollerWalker(double d_0, double theta_0, double omega, double phi,double phi_fr,double center_z, bool is_rollerWalk) {
	this->d_0 = d_0;
	this->theta_0 = theta_0;
	this->omega = omega;
	this->phi = phi;
	this->phi_fr = phi_fr;
	
	this->v_front = 0;
	this->v_side = 0;

	this->mu_t = VINYL_MU_t;
	this->mu_n = VINYL_MU_n;
	/*this->mu_t = CARPET_MU_t;
	this->mu_n = CARPET_MU_n;*/
	/*this->mu_t = 0.001;
	this->mu_n = 10;*/
	this->t = 0;
    this->is_rollerWalk = is_rollerWalk;
    if (is_rollerWalk) {//ローラウォーク時
		this->target_z = WHEEL_RADIUS;
		
	}
	else {//歩行時
		this->target_z =l4 +  WHEEL_THICKNESS;
	}

	this->center_z = center_z;
}
//各脚のゲッター
//左前脚
double BaseRollerWalker::getTheta1LF(){
	return theta_1_lf;
}
double BaseRollerWalker::getTheta2LF(){
	return theta_2_lf;
}
double BaseRollerWalker::getTheta3LF(){
	return theta_3_lf;
}
double BaseRollerWalker::getTheta4LF(){
	return theta_4_lf;
}
//左後ろ脚
double BaseRollerWalker::getTheta1LR(){
	return theta_1_lr;
}
double BaseRollerWalker::getTheta2LR(){
	return theta_2_lr;
}
double BaseRollerWalker::getTheta3LR(){
	return theta_3_lr;
}
double BaseRollerWalker::getTheta4LR(){
	return theta_4_lr;
}
//右後ろ脚
double BaseRollerWalker::getTheta1RR(){
	return theta_1_rr;
}
double BaseRollerWalker::getTheta2RR(){
	return theta_2_rr;
}
double BaseRollerWalker::getTheta3RR(){
	return theta_3_rr;
}
double BaseRollerWalker::getTheta4RR(){
	return theta_4_rr;
}
//右前脚
double BaseRollerWalker::getTheta1RF(){
	return theta_1_rf;
}
double BaseRollerWalker::getTheta2RF(){
	return theta_2_rf;
}
double BaseRollerWalker::getTheta3RF(){
	return theta_3_rf;
}
double BaseRollerWalker::getTheta4RF(){
	return theta_4_rf;
}

void BaseRollerWalker::calAndSetTheta(double t){
	this->t = t;
	//脚軌道に必要なパラメータの計算
	double d_lf  =d_front(t,d_0,omega);   //左前脚の長さ  
	double d_lr  =d_rear(t,d_0,omega);   //左後ろ脚の長さ  
	double d_rr  =d_rear(t,d_0,omega);   //右後ろ脚の長さ  
	double d_rf  =d_front(t,d_0,omega);   //右前脚の長さ

	if(is_rollerWalk){	//タイヤの中心がdとなるための計算
		d_lf = d_lf - l4 - WHEEL_THICKNESS/2;
		d_lr = d_lr - l4 - WHEEL_THICKNESS/2;
		d_rr = d_rr - l4 - WHEEL_THICKNESS/2;
		d_rf = d_rf - l4 - WHEEL_THICKNESS/2;
	}

	//左前脚
	theta_1_lf = theta_front(t,theta_0,omega)+PI/2;
	theta_2_lf = calTheta2(d_lf);
	theta_3_lf = calTheta3(d_lf,theta_2_lf);
	theta_4_lf = calTheta4(theta_2_lf,theta_3_lf,is_rollerWalk);
	//左後ろ脚
	theta_1_lr = theta_front(t,theta_0,omega)+PI/2;
	theta_2_lr = calTheta2(d_lr);
	theta_3_lr = calTheta3(d_lr,theta_2_lr);
	theta_4_lr = calTheta4(theta_2_lr,theta_3_lr,is_rollerWalk);
	//右後ろ脚
	theta_1_rr = -theta_front(t,theta_0,omega) - PI/2;
	theta_2_rr = calTheta2(d_rr);
	theta_3_rr = calTheta3(d_rr,theta_2_rr);
	theta_4_rr = calTheta4(theta_2_rr,theta_3_rr,is_rollerWalk);
	//右前脚
	theta_1_rf = -theta_front(t,theta_0,omega)- PI/2;
	theta_2_rf = calTheta2(d_rf);
	theta_3_rf = calTheta3(d_rf,theta_2_rf);
	theta_4_rf = calTheta4(theta_2_rf,theta_3_rf,is_rollerWalk);

	//std::cout << "時刻t: " << t << "  d_lf: " << d_lf << std::endl;
	//std::cout << "時刻t: " << t << " omega: "<<omega<<"  d_f:" << d_0 * (sin(omega * t + 3 * PI / 2) + 1) << std::endl;
	
}

// 前脚の脚軌道関数
double BaseRollerWalker::d_front(double t, double d_0, double omega) {
	double ret = d_ofset + d_0 * (sin(omega * t + 3 * PI / 2) + 1);
	//double ret = d_ofset + d_0 * (sin(omega * t) + 1);
	return ret;
}
double BaseRollerWalker::theta_front(double t, double theta_0, double omega) {
	double ret = -theta_0 * sin(omega * t + 3 * PI / 2 + phi);
	//double ret = -theta_0 * sin(omega * t+phi);
	return ret;
}
double BaseRollerWalker::dotD_front(double t,double d_0,double omega) {
	double ret = d_0 * omega * cos(omega * t + 3 * PI / 2);
	//double ret = d_0 * omega * cos(omega * t);
	return ret;
}
double BaseRollerWalker::dotTheta_front(double t,double theta_0,double omega) {
	double ret = -theta_0 * omega * cos(omega * t + 3 * PI / 2 + phi);
	//double ret = -theta_0 * omega * cos(omega * t + phi);
	return ret;
}

// 後ろ脚の脚軌道関数
double BaseRollerWalker::d_rear(double t, double d_0, double omega) {
	double ret = d_ofset + d_0 * (sin(omega * t + 3 * PI / 2+phi_fr) + 1);
	//double ret = d_ofset + d_0 * (sin(omega * t  + phi_fr) + 1);
	return ret;
}
double BaseRollerWalker::theta_rear(double t, double theta_0, double omega) {
	double ret = -theta_0 * sin(omega * t + 3 * PI / 2 + phi+ phi_fr);
	//double ret = -theta_0 * sin(omega * t  + phi+ phi_fr);
	return ret;
}
double BaseRollerWalker::dotD_rear(double t, double d_0, double omega) {
	double ret = d_0 * omega * cos(omega * t + 3 * PI / 2+ phi_fr);
	//double ret = d_0 * omega * cos(omega * t  + phi_fr);
	return ret;
}
double BaseRollerWalker::dotTheta_rear(double t, double theta_0, double omega) {
	double ret = -theta_0 * omega * cos(omega * t + 3 * PI / 2 + phi+ phi_fr);
	//double ret = -theta_0 * omega * cos(omega * t +  phi + phi_fr);
	return ret;
}

//θ適応則
void BaseRollerWalker::thetaAdaption(double v_d) {
	double tol = TOL;
	double theta_target_0;

	if (abs(v_d) > tol) {//加速が必要なときはθ_0は大きくなる
		theta_target_0 = THETA_MAX_0;
	}
	else {
		theta_target_0 = THETA_MIN_0;
	}

	theta_0 = theta_0 - K_THETA_0 * (theta_0 - theta_target_0);
}

double  BaseRollerWalker::empricalFormula() {//g(θ)
	double ret = 0;
	const double a_0 = 1.256;
	const double a_1 = -7.349;
	const double a_2 = 18.556;
	const double a_3 = -16.947;

	const double a_k[] = { a_0,a_1,a_2,a_3 };
	for (int k = 0; k <= 3; k++) {
		ret += a_k[k] * pow(theta_0, k);
	}

	return ret;
}

//ω適応則
void BaseRollerWalker::nominalOmegaAdaption(double v_d) {
	double omega_d = v_d / empricalFormula();	//ω_d = V_d/g(θ_0)
	double dot_omega = - K_OMEGA_d * (omega - omega_d);
	
	omega += dot_omega * DELTA_T; 
}

void BaseRollerWalker::ActualOmegaAdaption(double v_d) {
	double omega_d = v_d / empricalFormula();	//ω_d = V_d/g(θ_0)
	double dot_omega = -K_OMEGA_d * (omega - omega_d) - K_V * (v_front - v_d);

	omega += dot_omega * DELTA_T; //K_V=0ならノミナル適応速
}

//符号関数
double  BaseRollerWalker::sign(double input){
	if (input > 0) return 1;
	else if (input < 0) return -1;
	else return 0;
}

double BaseRollerWalker::calTheta2(double target_d) {
	double numerator = pow(target_d, 2) + pow((target_z - center_z), 2) + pow(LINK_2_LENGTH, 2) - pow(LINK_3_LENGTH, 2);
	double denominator = 2 * LINK_2_LENGTH * sqrt(pow(target_d, 2) + pow(target_z - center_z, 2));

	double ret = acos(numerator / denominator) + atan((target_z - center_z) /target_d ); //atan要チェック (acos2は[0,pi])

	return ret;
}

double BaseRollerWalker::calTheta3(double target_d, double theta2) {
	double numerator = (target_z - center_z) - LINK_2_LENGTH * sin(theta2);
	double denominator = target_d - LINK_2_LENGTH * cos(theta2);
	double ret = atan(numerator/denominator) - theta2 ; //atan要チェック

	return ret;
}

double BaseRollerWalker::calTheta4(double theta2,double theta3,bool is_rollerwalk) {
	double ret;
	if (is_rollerwalk) { //ローラウォーク時
		ret = - theta2 - theta3;
	}
	else {	//歩行時
		ret = - theta2 - theta3 - PI / 2;
	}
	
	return ret;
}



