#include<iostream>
#include<vector>
#include "BTC-USD2.h"
#include<math.h>
#include<fstream>
#include<omp.h>
#include <set>
#include<time.h>
using namespace std;

vector<double> macd;
vector<double> signal;


//#define REGULA_FALSI

void regula(double *x, double x0, double x1, double fx0, double fx1, int *itr)
{
    *x = x0 - ((x1 - x0) / (fx1 - fx0))*fx0;
    ++(*itr);
 
}
			
double macd_position(double time);
double signal_position(double time);

double table_macd_position(long unsigned int timeidx);
double table_signal_position(long unsigned int timeidx);



void builtTradePoints()
{

//Defininig Regulafalsi
#if defined(REGULA_FALSI)
	
	cout<<"Regula Falsi"<<endl;
	 cout.precision(15);
	set<double> ans;
	
	long int m;
	double amount=0.0;


	#pragma omp parallel for num_threads(4) shared(ans) private(m) reduction(+:amount)
	for(m=0;m<(2635-1);m+=1)
    {
		double x0,x1,x2,x3,allerr;
		int maxmitr=1000000;
		x0=(double)m;
	    	x1=(double)(2635-1);
		bool a=false;



//Setting up the error
		//allerr=0.00000000000001;

//Error of 1 second
        allerr=0.000001157;
		

		//allerr=1;
		//cout<<x0<<" "<<x1<<endl;

		int itr=0;


//Initializing Regulafalsi
	    regula (&x2, x0, x1,macd_position(x0)-signal_position(x0), macd_position(x1)-signal_position(x1), &itr);

        do 
        {
			if ((macd_position(x0)-signal_position(x0))*(macd_position(x2)-signal_position(x2)) < 0)
			    x1=x2;
			 else
			    x0=x2;

			 regula (&x3, x0, x1, (macd_position(x0)-signal_position(x0)), (macd_position(x1)-signal_position(x1)), &itr); 
			
			if (fabs(x3-x2) < allerr)
			{
			  
			  
			  if(x3>0 && x3<=macd.size() && ans.find(x3)==ans.end())
                {
			    	
					
			    	a=true;
					

					if(macd_position(x3) > signal_position(x3))
						{
							amount-=close[(int)round(x3)];
							
						}
					else if(macd_position(x3)<signal_position(x3))
						{
							amount+=close[(int)round(x3)];
							
						}

					ans.insert(x3);
					

			    	itr=5000000;
			    }
			
			}
				x2=x3;
		} while (itr<maxmitr);


		if(!a)
        {
			a=false;
		} 
	}
	    cout << "Total crosses = " << ans.size() << endl;

	    cout<<amount<<endl;
#else

//Defining the brute forceway
#define TIMESPERDAY (86400)

   cout<<"Brute Force"<<endl;
    int dayidx, cross_cnt=0;

    cout.precision(15);
	
	double amount=0.0;
	
//main sign change program
	#pragma omp parallel for num_threads(4) private(dayidx) shared(cross_cnt) reduction(+:amount) 
	for(dayidx=0;dayidx<(2635-1)*TIMESPERDAY;dayidx++)
    {
		
		
		 double function_error1=0.0, function_error2=0.0, day, day_step=1.0/((double)TIMESPERDAY);

//Linnear interpolation
		day=(double)dayidx/TIMESPERDAY;

        function_error1 = macd_position(day)-signal_position(day);
        function_error2 = macd_position(day+day_step)-signal_position(day+day_step);

        // Crosses X going down
        if(function_error1 > 0.0 && function_error2 < 0.0)
        {
       //     cout << "Day=" << day << "-" << (day+day_step) << " pos-to-neg crossing between " << function_error1 << " and " << function_error2 << endl;
			amount+=close[(int)round(day)];
            cross_cnt++;
        }
        // Crosses X going up
        else if(function_error1 < 0.0 && function_error2 > 0.0)
        {
     //       cout << "Day=" << day << "-" << (day+day_step) << " neg-to-pos crossing between " << function_error1 << " and " << function_error2 << endl;
			amount+= (-close[(int)round(day)]);
            cross_cnt++;
        }
        else 
        {
            //cout << "ferror=" << function_error1 << endl;
        }
    }

    cout << "Total crosses = " << cross_cnt << endl;
	cout<<amount<<endl;
#endif


}



//print the macd and signal line out in a file for excel to filter
void print_csv(vector<double> macd,vector<double> signal,int size,string filename){
	
	ofstream myfile;
	myfile.open(filename);
	int i=0;
	for(i =0;i<size;i++){
		myfile << macd[i]<<","<<signal[i]<<"\n";
	}
	myfile.close();
}


int main(int argc, char** argv){


//variable definition
	double fstart,fend;
	struct timespec start,end;
	clock_gettime(CLOCK_MONOTONIC, &start); 

	double alpha,beta;
	int period,size;
	size=sizeof(close)/sizeof(double);
	int threads=4;
	period=26;

	alpha=(2/(1.0+period));
	beta=(1.0-alpha);


//Calculating ema for 26 days using the EMA expansion for close price
	vector<double> t26emaTable(size,0.0);
	int t=0;
	#pragma omp parallel for num_threads(threads) private(t)
	for(t=0;t<size;t++){

	if(t<period){
		t26emaTable.at(t)=0;
		continue;
	}
		for(int j=t;j>=t-period;j--){
			t26emaTable.at(t)+=(alpha*pow(beta,t-j)*close[j]);
		}

	}



	period=12;

	alpha=(2/(1.0+period));
	beta=(1.0-alpha);

	vector<double> t12emaTable(size,0.0);
	
	t=0;
//Calculating ema for 12 days using the EMA expansion for close price
	#pragma omp parallel for num_threads(threads) private(t)
	for(t=0;t<size;t++){

	if(t<period){
		t12emaTable.at(t)=0;
		continue;
	}
		for(int j=t;j>=t-period;j--){
			t12emaTable.at(t)+=(alpha*pow(beta,t-j)*close[j]);	
		}

	}


	vector<double> macd_line;
	for(int i=0;i<size;i++){
		macd_line.push_back(t12emaTable[i]-t26emaTable[i]);
	}

	period=9;

	alpha=(2/(1.0+period));
	beta=(1.0-alpha);

	vector<double> t9emaTable(size,0.0);

	t=0;
	
//Claculating the EMA for 9 day over MACD line using EMA expansion
	#pragma omp parallel for num_threads(threads) private(t)
	for(t=0;t<size;t++){

	if(t<period){
		t9emaTable.at(t)=0;
		continue;
	}
		for(int j=t;j>=t-period;j--){
			t9emaTable.at(t)+=(alpha*pow(beta,t-j)*macd_line[j]);	
		}

	}

	
//assigning data to global variables for easy access during multithreading
	macd=macd_line;
	signal=t9emaTable;
	
	
//Calling regulafalsi and signchange bruteforce 
	builtTradePoints();
	
	clock_gettime(CLOCK_MONOTONIC, &end);
	fstart=start.tv_sec + (start.tv_nsec / 1000000000.0);
	fend=end.tv_sec + (end.tv_nsec / 1000000000.0);
	printf("Time to run %lf",(fend-fstart));
	
	
	print_csv(macd_line,t9emaTable,size,"macd.csv");


	return 0;

}
//fetching the table values for MACD
double table_position(long unsigned int timeidx)
{
    long unsigned int tsize = macd.size();

    if(timeidx > tsize)
    {
        //printf("timeidx=%lu exceeds table size = %lu and range %lu to %lu\n", timeidx, tsize, (long unsigned int)0, tsize-1);
        //exit(-1);

        timeidx=tsize;
    }

    return macd[timeidx];
}

//fetching the table values for Signal
double table_signal_position(long unsigned int timeidx)
{
    long unsigned int tsize = signal.size();


    if(timeidx > tsize)
    {
        //printf("timeidx=%lu exceeds signal table size = %lu and range %lu to %lu\n", timeidx, tsize, (long unsigned int)0, tsize-1);
        //exit(-1);
        timeidx=tsize;
    }

    return signal[timeidx];
}

//Getting the interpolated value from the original table value for MACD
double macd_position(double time)
{

    int timeidx = (long unsigned int)time;


    int timeidx_next = ((long unsigned int)time)+1;


    double delta_t = time - (double)((long unsigned int)time);

    return ( 
               table_position(timeidx) + ( (table_position(timeidx_next) - table_position(timeidx)) * delta_t)
           );
}

//Getting the interpolated value for original table for Signal
double signal_position(double time)
{

    int timeidx = (long unsigned int)time;

    int timeidx_next = ((long unsigned int)time)+1;


    double delta_t = time - (double)((long unsigned int)time);

    return ( 
               table_signal_position(timeidx) + ( (table_signal_position(timeidx_next) - table_signal_position(timeidx)) * delta_t)
           );
}
