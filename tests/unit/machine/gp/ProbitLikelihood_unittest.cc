/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 2013 Roman Votyakov
 */

#include <shogun/lib/config.h>

#ifdef HAVE_EIGEN3

#include <shogun/labels/BinaryLabels.h>
#include <shogun/machine/gp/ProbitLikelihood.h>
#include <gtest/gtest.h>

using namespace shogun;

TEST(ProbitLikelihood,evaluate_means)
{
	// create some easy data:
	// mu(x) approximately equals to 3*sin(sin(x^2)*sin(sin(2*x)))
	index_t n=10;

	SGVector<float64_t> s2(n);
	SGVector<float64_t> mu(n);

	s2[0]=0.1;
	s2[1]=0.2;
	s2[2]=1.0;
	s2[3]=0.7;
	s2[4]=0.3;
	s2[5]=0.1;
	s2[6]=1.0;
	s2[7]=0.5;
	s2[8]=0.7;
	s2[9]=0.4;

	mu[0]=0.889099;
	mu[1]=0.350840;
	mu[2]=-2.116356;
	mu[3]=-0.184742;
	mu[4]=0.182117;
	mu[5]=-1.108930;
	mu[6]=-0.062437;
	mu[7]=0.482987;
	mu[8]=-0.149445;
	mu[9]=0.106952;

	// probit likelihood
	CProbitLikelihood* likelihood=new CProbitLikelihood();

	mu=likelihood->evaluate_means(mu, s2);

	// comparison of the first moment with result from GPML package
	EXPECT_NEAR(mu[0], 0.603407542, 1E-9);
	EXPECT_NEAR(mu[1], 0.251237577, 1E-9);
	EXPECT_NEAR(mu[2], -0.865473904, 1E-9);
	EXPECT_NEAR(mu[3], -0.112675636, 1E-9);
	EXPECT_NEAR(mu[4], 0.126904007, 1E-9);
	EXPECT_NEAR(mu[5], -0.709635922, 1E-9);
	EXPECT_NEAR(mu[6], -0.035214864, 1E-9);
	EXPECT_NEAR(mu[7], 0.306682687, 1E-9);
	EXPECT_NEAR(mu[8], -0.091252946, 1E-9);
	EXPECT_NEAR(mu[9], 0.072023442, 1E-9);

	// clean up
	SG_UNREF(likelihood);
}

TEST(ProbitLikelihood,evaluate_variances)
{
	// create some easy data:
	// mu(x) approximately equals to 3*sin(sin(x^2)*sin(sin(2*x)))
	index_t n=10;

	SGVector<float64_t> s2(n);
	SGVector<float64_t> mu(n);

	s2[0]=0.1;
	s2[1]=0.2;
	s2[2]=1.0;
	s2[3]=0.7;
	s2[4]=0.3;
	s2[5]=0.1;
	s2[6]=1.0;
	s2[7]=0.5;
	s2[8]=0.7;
	s2[9]=0.4;

	mu[0]=0.889099;
	mu[1]=0.350840;
	mu[2]=-2.116356;
	mu[3]=-0.184742;
	mu[4]=0.182117;
	mu[5]=-1.108930;
	mu[6]=-0.062437;
	mu[7]=0.482987;
	mu[8]=-0.149445;
	mu[9]=0.106952;

	// logit likelihood
	CProbitLikelihood* likelihood=new CProbitLikelihood();

	s2=likelihood->evaluate_variances(mu, s2);

	// comparison of the second moment with result from GPML package
	EXPECT_NEAR(s2[0], 0.635899337, 1E-9);
	EXPECT_NEAR(s2[1], 0.936879679, 1E-9);
	EXPECT_NEAR(s2[2], 0.250954920, 1E-9);
	EXPECT_NEAR(s2[3], 0.987304201, 1E-9);
	EXPECT_NEAR(s2[4], 0.983895372, 1E-9);
	EXPECT_NEAR(s2[5], 0.496416856, 1E-9);
	EXPECT_NEAR(s2[6], 0.998759913, 1E-9);
	EXPECT_NEAR(s2[7], 0.905945729, 1E-9);
	EXPECT_NEAR(s2[8], 0.991672899, 1E-9);
	EXPECT_NEAR(s2[9], 0.994812623, 1E-9);

	// clean up
	SG_UNREF(likelihood);
}

TEST(ProbitLikelihood,get_log_probability_f_sum)
{
	// create some easy data:
	// f(x) approximately equals to 3*sin(sin(x^2)*sin(sin(2*x))), y = sign(f(x))
	index_t n=10;

	SGVector<float64_t> lab(n);
	SGVector<float64_t> func(n);

	lab[0]=1.0;
	lab[1]=1.0;
	lab[2]=-1.0;
	lab[3]=-1.0;
	lab[4]=1.0;
	lab[5]=-1.0;
	lab[6]=-1.0;
	lab[7]=1.0;
	lab[8]=-1.0;
	lab[9]=1.0;

	func[0]=0.889099;
	func[1]=0.350840;
	func[2]=-2.116356;
	func[3]=-0.184742;
	func[4]=0.182117;
	func[5]=-1.108930;
	func[6]=-0.062437;
	func[7]=0.482987;
	func[8]=-0.149445;
	func[9]=0.106952;

	// shogun representation of labels
	CBinaryLabels* labels=new CBinaryLabels(lab);

	// probit likelihood
	CProbitLikelihood* likelihood=new CProbitLikelihood();

	float64_t lp=SGVector<float64_t>::sum(likelihood->get_log_probability_f(labels, func));

	// comparison of log likelihood with result from GPML package
	EXPECT_NEAR(lp, -4.147705557, 1E-9);

	// clean up
	SG_UNREF(likelihood);
	SG_UNREF(labels);
}

TEST(ProbitLikelihood,get_log_probability_derivative_f)
{
	// create some easy data:
	// f(x) approximately equals to 3*sin(sin(x^2)*sin(sin(2*x))), y = sign(f(x))
	index_t n=10;

	SGVector<float64_t> lab(n);
	SGVector<float64_t> func(n);

	lab[0]=1.0;
	lab[1]=1.0;
	lab[2]=-1.0;
	lab[3]=-1.0;
	lab[4]=1.0;
	lab[5]=-1.0;
	lab[6]=-1.0;
	lab[7]=1.0;
	lab[8]=-1.0;
	lab[9]=1.0;

	func[0]=0.889099;
	func[1]=0.350840;
	func[2]=-2.116356;
	func[3]=-0.184742;
	func[4]=0.182117;
	func[5]=-1.108930;
	func[6]=-0.062437;
	func[7]=0.482987;
	func[8]=-0.149445;
	func[9]=0.106952;

	// shogun representation of labels
	CBinaryLabels* labels=new CBinaryLabels(lab);

	// probit likelihood
	CProbitLikelihood* likelihood=new CProbitLikelihood();

	SGVector<float64_t> dlp=likelihood->get_log_probability_derivative_f(labels, func, 1);
	SGVector<float64_t> d2lp=likelihood->get_log_probability_derivative_f(labels, func, 2);
	SGVector<float64_t> d3lp=likelihood->get_log_probability_derivative_f(labels, func, 3);

	// comparison of log likelihood derivatives with result from GPML package
	EXPECT_NEAR(dlp[0], 0.330485099, 1E-9);
	EXPECT_NEAR(dlp[1], 0.588766191, 1E-9);
	EXPECT_NEAR(dlp[2], -0.043234619, 1E-9);
	EXPECT_NEAR(dlp[3], -0.684114614, 1E-9);
	EXPECT_NEAR(dlp[4], 0.685675731, 1E-9);
	EXPECT_NEAR(dlp[5], -0.249014899, 1E-9);
	EXPECT_NEAR(dlp[6], -0.758565533, 1E-9);
	EXPECT_NEAR(dlp[7], 0.517941276, 1E-9);
	EXPECT_NEAR(dlp[8], -0.705243109, 1E-9);
	EXPECT_NEAR(dlp[9], 0.731067061, 1E-9);

	EXPECT_NEAR(d2lp[0], -0.403054372, 1E-9);
	EXPECT_NEAR(d2lp[1], -0.553208359, 1E-9);
	EXPECT_NEAR(d2lp[2], -0.093369078, 1E-9);
	EXPECT_NEAR(d2lp[3], -0.594397508, 1E-9);
	EXPECT_NEAR(d2lp[4], -0.595024415, 1E-9);
	EXPECT_NEAR(d2lp[5], -0.338148513, 1E-9);
	EXPECT_NEAR(d2lp[6], -0.622784225, 1E-9);
	EXPECT_NEAR(d2lp[7], -0.518422069, 1E-9);
	EXPECT_NEAR(d2lp[8], -0.602762899, 1E-9);
	EXPECT_NEAR(d2lp[9], -0.612648132, 1E-9);

	EXPECT_NEAR(d3lp[0], 0.294277068, 1E-9);
	EXPECT_NEAR(d3lp[1], 0.256742186, 1E-9);
	EXPECT_NEAR(d3lp[2], -0.162441143, 1E-9);
	EXPECT_NEAR(d3lp[3], -0.238967614, 1E-9);
	EXPECT_NEAR(d3lp[4], 0.238675932, 1E-9);
	EXPECT_NEAR(d3lp[5], -0.294376167, 1E-9);
	EXPECT_NEAR(d3lp[6], -0.225164541, 1E-9);
	EXPECT_NEAR(d3lp[7], 0.269474219, 1E-9);
	EXPECT_NEAR(d3lp[8], -0.235025555, 1E-9);
	EXPECT_NEAR(d3lp[9], 0.230230621, 1E-9);

	// clean up
	SG_UNREF(likelihood);
	SG_UNREF(labels);
}

#endif /* HAVE_EIGEN3 */
