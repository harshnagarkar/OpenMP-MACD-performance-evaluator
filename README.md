# MACD-performance-evaluator

Final Program Objectives:
The learning objectives for the final parallel program are:
•	Choose a numerical and parallel problem that interests you and that is relevant to the class (studied or related to what we have studied), explain the problem well, and describe your proposed numerical method (algorithm), the features your program will have, and how you plan to design and program it using MPI, OpenMP, Pthreads, or some hybrid of the 3 methods (combining two or more), or some new method (e.g. CUDA).

•	Implement a sequential program that solves your problem and time it accurately for variously sized problem scenarios as well as describing the algorithmic complexity.

•	Design and implement a parallel solution using one of the parallel programming methods we studied on a multi-core / multi-node system that provides shared memory or distributed memory processor scaling. After making parallel, test this version still produces correct and accurate results for precision used.

•	Show that your parallel design and implementation speeds-up your solution and that it scales with number of processor cores used. Carefully time, identify sections that run in parallel and those that are sequential (use code-level timestamps if more accurate than using command line “time” function – most often this is required).

•	Provide a code walk-through of your parallel solution, demonstrate build, and run, and describe how it works.

## Final program description in terms of major goals and objectives, and problem solved. E.g., this program speeds-up large vector matrix multiplication.

The main goal of this project to was to implement “Moving Average Converge and Divergence” indicator or “MACD” over an equity’s chart parallelly to estimate total earning over the historical data of a particular stock. MACD is an oscillator that is commonly used by the stockbroker to calculate the momentum of a stock. An oscillator a technical analysis used to calculate the high and low bands between extremes.  A momentum of a stock refers to an inertia of a price trend to continue either rising or falling for a particular length of time. Momentum usually considers both price as well as the volume during its calculation. Therefore, using such an indicator we could execute trade and see the total earning over a period.
	MACD indicator line is usually calculated by subtracting the 26 days exponential moving average from 12 days exponential moving average. In order to generate a trade signal in algorithm, a signal line is also plotted. According to yahoo finance the signal line for MACD is 9 days exponential moving average of MACD indicator line. The cross over of this signal line and MACD line indicates either a buy or sell trade indicator.

[5 pts] The value of your solution and applications of it – e.g., often used for linear systems, such as engineering problems in mechanics and chemical engineering (cite an example from class or that you research on the web).
	The MACD indicator is commonly used by a equities analyst to generate either a buy or sell trade signal for a particular trade. It is usually used in short term trade. Although there might few exceptions to this like SP500 since July of this year, where we see a weak signal, most equities follow the MACD norm. This algorithm potentially helps traders to delegate some of their risks on such indicator to avoid huge losses.

## What numerical methods and algorithms are used and what type of math is required? Please indicate and then describe your use and objective for using the method.  

Brute force sign change
Regula falsi method 

Implemented on 
Over piece wise linear interpolation for intraday prices.

## Parallelized by

OpenMP compiler optimization

## Sequential solution and computation time.

After unzipping the provided folder, 
*	Sequential code activation is stock_macd.cpp. 
 In order to activate the sign change code, comment the top line 
“#define REGULA_FALSI”
*	In order to activate the regula falsi code, uncomment the top line 
“#define REGULA_FALSI”
*	Sequential code making
	make stock_macd
*	Running the Sequential code
	
  ./stock_macd


The average runtime of the algorithm on o244-27 is 
*	Brute Force
    28.675091 sec = 0.4779 min
*	Regula Falsi
    680.145505 sec = 11.3357 min



## Parallel design and solution with computation time.

After unzipping the provided folder, 
*	Parallel code activation is stock_macd.cpp. 
In order to activate the sign change code, comment the top line 
“#define REGULA_FALSI”
* In order to activate the regula falsi code, uncomment the top line 
“#define REGULA_FALSI”
*	Parallel code making
	make stock_macd_omp
*	Running the Parallel code
o	./stock_macd_omp

*	Brute Force
	12.971248 sec =  0.2161 min
*	RegulaFalsi
  286.340167 sec = 4.77233 min
