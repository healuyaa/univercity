#include <iostream>
#include <cmath>
#include <iomanip>

#define N 3 // inputs INS
#define ed 30 // educate
#define fc 15 // forecasting
#define sp 0.1 // speed educate

double F(double x)
{
	double a = 1.0, b = 5.0, d = 0.1;
	return (a * sin(b * x) + d);
}

int main()
{
	double x[(ed + fc)]; // input x
	double t[(ed + fc)]; // etalon value
	double y[(ed + fc)]; // return value

	for (int i = 0; i < ed + fc; i++) {
		x[i] = (double)i * 0.1;// value step 0.1
		t[i] = F(x[i]); // tab func step 0.1
	}

	for (int i = 0; i < N; i++)
		y[i] = t[i];

	double W[N]; //value coef
	double T; // treeshold	

	for (int i = 0; i < N; i++) {
		W[i] = (double)((rand()%3)/3.4 + 1.2);// init
		std::cout <<" W"<<"[" << i << "]"<< "= " << W[i] << std::endl;
	}
	T =  (double)((rand() % 3) / 3.4 + 1.2);
	// educate
	double Em = 1e-6; // min error
	double E; // valid error
	std::cout << "Educate:" << std::endl << std::endl;                 
		do
		{
			E = 0;
			std::cout << std::setw(18) << "Etalon value" << " | " << std::setw(15) << "Return value" << " | " << std::setw(11) << "Error" << std::endl;
			for (int i = 0; i < ed - N; i++)
			{
				y[N + i] = 0;
				for (int j = 0; j < N; j++)
					y[N + i] += W[j] * t[i + j]; // formula 1.2
				y[N + i] -= T;
				for (int j = 0; j < N; j++)
					W[j] -= sp * (y[N + i] - t[N + i]) * t[i + j]; // formuls 1.7, 1.8
				T += sp * (y[N + i] - t[N + i]);
				E += pow((y[N + i] - t[N + i]), 2.0) / 2.0; // formula 1.3
				std::cout << std::setw(18) << t[N+i] << " | " << std::setw(15) << y[N+i] << " | " << std::setw(18) << E << std::endl;
			}
			//cout << E << endl;
		} while (E > Em);

	// forecasting
	std::cout << std::setw(25) << std::endl << "Forecasting:" << std::endl << std::endl;
	std::cout << std::setw(15) << " x: " << " | " << std::setw(15) << " y(valid) " << " | " << std::setw(20) << " y(predicted): " << " | " << std::setw(15) << " Error: " << std::endl;
	for (int i = ed; i < ed + fc; i++)
	{
		y[i] = 0;
		for (int k = 0; k < N; k++) 
			y[i] += W[k] * t[i - N + k];
		y[i] -= T;
			std::cout << std::setw(15) << x[i] << " | " << std::setw(15) << t[i] << " | " << std::setw(20) << y[i] << " | " << std::setw(15) << fabs(t[i] - y[i]) << std::endl;		
	}
    system("pause");
	return 0;
}
