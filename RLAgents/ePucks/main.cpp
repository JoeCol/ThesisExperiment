#include <stdio.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <iomanip>
#include <numeric>
#include "playerSup.h"
#include "support.h"
#include "mat.h"

std::vector<RLEmotional*> createAgents(int numOfRobots, StateMode m, bool staghunt, MoodAffectMode mam, double moodAffect);
void resetAgents(std::vector<RLEmotional*> agents);
double getOverallAverage(std::vector<RLEmotional*> agents);
double stdDevDouble(std::vector<double> doubles, int size);
double getDblDiff(double i, double j);
double getOverallTotal(std::vector<RLEmotional*> agents);
double getMean(std::vector<double> doubles);
double getConfInterval(double z, double SD, double size);
std::map<boost::uuids::uuid, RLEmotional*> assignAgents(std::vector<RLEmotional*> agents, std::set<boost::uuids::uuid> IDs);

int main(int argc, char **argv)
{
    MATFile* pmat;
    mxArray* average;
    mxArray* total;
    mxArray* coop;
    mxArray* defect;
    mxArray* nonmutual;
    mxArray* confidenceInterval;
    srand((int)time(NULL));
	std::pair<std::set<boost::uuids::uuid>, std::vector<ePuckPosition*> > results;
    std::vector<std::vector<RLEmotional*>> agents;
    std::map<int, bool> convergence;
	std::map<boost::uuids::uuid, RLEmotional*> epucks;
    
    int numberOfRobots = 108;
    int simTimeMilli = 600000;
    int numOfRuns = 10;
    StateMode m;
    bool staghunt = false;
    MoodAffectMode mam;
    double moodAffect;
    
    std::string worldFile = "";
    std::string arenaFilePath = "";
    std::string arenaName = "";
    std::string saveFolder = "";
    std::string matDirectory = "";
    
    getArguments(argc, argv, arenaFilePath, worldFile, arenaName, saveFolder, matDirectory, numOfRuns, m, staghunt, mam, moodAffect);
    std::vector<double> averages;
    std::vector<double> totals;
    std::vector<double> coops;
    std::vector<double> defects;
    std::vector<double> nonmutuals;
    
    std::map<int, std::vector<double>> runCOOP;
    std::map<int, std::vector<double>> runDEFECT;
    std::map<int, std::vector<double>> runNONMUTUAL;
    std::map<int, std::vector<double>> runAVERAGE;
    std::map<int, std::vector<double>> runTOTAL;
    
    //Confidence interval checking
    /*bool confidence = true;
    double coopmean;
    double defectmean;
    double nonmutualmean;
    double averagemean;
    double totalmean;
    
    double coopsd;
    double defectsd;
    double nonmutualsd;
    double averagesd;
    double totalsd;
    int i = 0;
    while (confidence)
    {
        i++;
        std::vector<RLEmotional*> a = createAgents(numberOfRobots, m, staghunt, mam, moodAffect);
        generatePlayerFiles(numberOfRobots, simTimeMilli, worldFile, arenaName, arenaFilePath);
        results = runStageSim(worldFile);
        epucks = assignAgents(a, results.first);
        doGames(saveFolder, numberOfRobots, epucks, results.second, simTimeMilli, staghunt);
        runCOOP.push_back(getTotalFor(COOP, true) / getTotalGames());
        runDEFECT.push_back(getTotalFor(DEFECT, true) / getTotalGames());
        runNONMUTUAL.push_back(getTotalFor(COOP, false) / getTotalGames());
        runAVERAGE.push_back(getOverallAverage(a));
        runTOTAL.push_back(getOverallTotal(a));
        
        coopmean = getMean(runCOOP);
        defectmean = getMean(runDEFECT);
        nonmutualmean = getMean(runNONMUTUAL);
        averagemean = getMean(runAVERAGE);
        totalmean = getMean(runTOTAL);
        
        coopsd = stdDevDouble(runCOOP, (int)runCOOP.size());
        defectsd = stdDevDouble(runDEFECT, (int)runDEFECT.size());
        nonmutualsd = stdDevDouble(runNONMUTUAL, (int)runNONMUTUAL.size());
        averagesd = stdDevDouble(runAVERAGE, (int)runAVERAGE.size());
        totalsd = stdDevDouble(runTOTAL, (int)runTOTAL.size());
        
        
        std::cout << "Coop Mean " << coopmean << "±" << getConfInterval(0.90, coopsd, (int)runCOOP.size()) << " 90% " << getConfInterval(0.95, coopsd, (int)runCOOP.size()) << " 95% " << getConfInterval(0.99, coopsd, (int)runCOOP.size()) << " 99% using " << i << "runs" << std::endl;
        
        std::cout << "Defect Mean " << defectmean << "±" << getConfInterval(0.90, defectsd, (int)runDEFECT.size()) << " 90% " << getConfInterval(0.95, defectsd, (int)runDEFECT.size()) << " 95% " << getConfInterval(0.99, defectsd, (int)runDEFECT.size()) << " 99% using " << i << "runs" << std::endl;
        
        std::cout << "Nonmutual Mean " << nonmutualmean << "±" << getConfInterval(0.90, nonmutualsd, (int)runNONMUTUAL.size()) << " 90% " << getConfInterval(0.95, nonmutualsd, (int)runNONMUTUAL.size()) << " 95% " << getConfInterval(0.99, nonmutualsd, (int)runNONMUTUAL.size()) << " 99% using " << i << "runs" << std::endl;
        
        std::cout << "average Mean " << averagemean << "±" << getConfInterval(0.90, averagesd, (int)runAVERAGE.size()) << " 90% " << getConfInterval(0.95, averagesd, (int)runAVERAGE.size()) << " 95% " << getConfInterval(0.99, averagesd, (int)runAVERAGE.size()) << " 99% using " << i << "runs" << std::endl;
        
        std::cout << "total Mean " << totalmean << "±" << getConfInterval(0.90, totalsd, (int)runTOTAL.size()) << " 90% " << getConfInterval(0.95, totalsd, (int)runTOTAL.size()) << " 95% " << getConfInterval(0.99, totalsd, (int)runTOTAL.size()) << " 99% using " << i << "runs" << std::endl << std::endl;
        //Generate confidence intervals
    }
    */
    for (int i = 0; i < numOfRuns; i++)
    {
        std::vector<RLEmotional*> a = createAgents(numberOfRobots, m, staghunt, mam, moodAffect);
        agents.push_back(a);
        convergence[i] = false;
    }
    
    int runs = 0;
    bool converged = false;
    std::vector<double> finalCoop;
    std::vector<double> finalDefect;
    std::vector<double> finalNonmutual;
    std::vector<double> finalTotal;
    std::vector<double> finalAverage;
    while (!converged) //While not converged
    {
        double sumAverage = 0.0;
        double sumTotals = 0.0;
        double sumCOOPS = 0.0;
        double sumDefects = 0.0;
        double sumNonmutual = 0.0;
        
        finalCoop.clear();
        finalDefect.clear();
        finalNonmutual.clear();
        finalTotal.clear();
        finalAverage.clear();
        
        for (int i = 0; i < numOfRuns; i++) //For each run need tp get point
        {
            resetAgents(agents[i]);
            resetTotals();
            if (!convergence[i]) // if not converged
            {
                generatePlayerFiles(numberOfRobots, simTimeMilli, worldFile, arenaName, arenaFilePath);
                results = runStageSim(worldFile);
                epucks = assignAgents(agents[i], results.first);
                doGames(saveFolder, numberOfRobots, epucks, results.second, simTimeMilli, staghunt);
                runCOOP[i].push_back(getTotalFor(COOP, true) / getTotalGames());
                runDEFECT[i].push_back(getTotalFor(DEFECT, true) / getTotalGames());
                runNONMUTUAL[i].push_back(getTotalFor(COOP, false) / getTotalGames());
                runAVERAGE[i].push_back(getOverallAverage(agents[i]));
                runTOTAL[i].push_back(getOverallTotal(agents[i]));
                
                //Check if converged
                bool coopConv = getDblDiff(getMean(std::vector<double>(runCOOP[i].end() - 25, runCOOP[i].end())), getMean(std::vector<double>(runCOOP[i].end() - 5, runCOOP[i].end()))) < 0.005;
                bool defectConv = getDblDiff(getMean(std::vector<double>(runDEFECT[i].end() - 25, runDEFECT[i].end())), getMean(std::vector<double>(runDEFECT[i].end() - 5, runDEFECT[i].end()))) < 0.005;
                bool nonmutualConv = getDblDiff(getMean(std::vector<double>(runNONMUTUAL[i].end() - 25, runNONMUTUAL[i].end())), getMean(std::vector<double>(runNONMUTUAL[i].end() - 5, runNONMUTUAL[i].end()))) < 0.005;
                convergence[i] = coopConv && defectConv && nonmutualConv;
            }
            else
            {
                runCOOP[i].push_back(runCOOP[i].back());
                runDEFECT[i].push_back(runDEFECT[i].back());
                runNONMUTUAL[i].push_back(runNONMUTUAL[i].back());
                runAVERAGE[i].push_back(runAVERAGE[i].back());
                runTOTAL[i].push_back(runTOTAL[i].back());
            }
            sumAverage += runAVERAGE[i].back();
            sumTotals += runTOTAL[i].back();
            sumCOOPS += runCOOP[i].back();
            sumDefects += runDEFECT[i].back();
            sumNonmutual += runNONMUTUAL[i].back();
            
            finalCoop.push_back(runCOOP[i].back());
            finalDefect.push_back(runDEFECT[i].back());
            finalNonmutual.push_back(runNONMUTUAL[i].back());
            finalTotal.push_back(runTOTAL[i].back());
            finalAverage.push_back(runAVERAGE[i].back());
        }
        std::cout << "Done " << runs << std::endl;
        
        averages.push_back(sumAverage / numOfRuns);
        totals.push_back(sumTotals / numOfRuns);
        coops.push_back(sumCOOPS / numOfRuns);
        defects.push_back(sumDefects / numOfRuns);
        nonmutuals.push_back(sumNonmutual / numOfRuns);
        
        bool allConvergence = true;
        for (int i = 0; i < numOfRuns; i++)
        {
            if (!convergence[i])
            {
                allConvergence = false;
                break;
            }
        }
        converged = allConvergence;
        runs++;
    }
    //
    average = mxCreateDoubleMatrix(runs, 1, mxREAL);
    total = mxCreateDoubleMatrix(runs, 1, mxREAL);
    coop = mxCreateDoubleMatrix(runs, 1, mxREAL);
    defect = mxCreateDoubleMatrix(runs, 1, mxREAL);
    nonmutual = mxCreateDoubleMatrix(runs, 1, mxREAL);
    confidenceInterval = mxCreateDoubleMatrix_700(5, 3, mxREAL);
    double confidence[15];
    confidence[0] = getConfInterval(0.9, stdDevDouble(finalCoop, numOfRuns), numOfRuns);
    confidence[5] = getConfInterval(0.95, stdDevDouble(finalCoop, numOfRuns), numOfRuns);
    confidence[10] = getConfInterval(0.99, stdDevDouble(finalCoop, numOfRuns), numOfRuns);
    
    confidence[1] = getConfInterval(0.9, stdDevDouble(finalDefect, numOfRuns), numOfRuns);
    confidence[6] = getConfInterval(0.95, stdDevDouble(finalDefect, numOfRuns), numOfRuns);
    confidence[11] = getConfInterval(0.99, stdDevDouble(finalDefect, numOfRuns), numOfRuns);
    
    confidence[2] = getConfInterval(0.9, stdDevDouble(finalNonmutual, numOfRuns), numOfRuns);
    confidence[7] = getConfInterval(0.95, stdDevDouble(finalNonmutual, numOfRuns), numOfRuns);
    confidence[12] = getConfInterval(0.99, stdDevDouble(finalNonmutual, numOfRuns), numOfRuns);
    
    confidence[3] = getConfInterval(0.9, stdDevDouble(finalTotal, numOfRuns), numOfRuns);
    confidence[8] = getConfInterval(0.95, stdDevDouble(finalTotal, numOfRuns), numOfRuns);
    confidence[13] = getConfInterval(0.99, stdDevDouble(finalTotal, numOfRuns), numOfRuns);
    
    confidence[4] = getConfInterval(0.9, stdDevDouble(finalAverage, numOfRuns), numOfRuns);
    confidence[9] = getConfInterval(0.95, stdDevDouble(finalAverage, numOfRuns), numOfRuns);
    confidence[14] = getConfInterval(0.99, stdDevDouble(finalAverage, numOfRuns), numOfRuns);
    
    memcpy((void *)(mxGetPr(average)), (void *)averages.data(), sizeof(double) * runs);
    memcpy((void *)(mxGetPr(total)), (void *)totals.data(), sizeof(double) * runs);
    memcpy((void *)(mxGetPr(coop)), (void *)coops.data(), sizeof(double) * runs);
    memcpy((void *)(mxGetPr(defect)), (void *)defects.data(), sizeof(double) * runs);
    memcpy((void *)(mxGetPr(nonmutual)), (void *)nonmutuals.data(), sizeof(double) * runs);
    memcpy((void *)(mxGetPr(confidenceInterval)), (void *)confidence, sizeof(double) * 15);
    
    pmat = matOpen(saveFolder.c_str(), "w");
    matPutVariable(pmat, "Averages", average);
    matPutVariable(pmat, "Totals", total);
    matPutVariable(pmat, "Coops", coop);
    matPutVariable(pmat, "Defects", defect);
    matPutVariable(pmat, "Nonmutual", nonmutual);
    matPutVariable(pmat, "Confidence", confidenceInterval);
    mxDestroyArray(average);
    mxDestroyArray(total);
    matClose(pmat);
    
	return 0;
}

void resetAgents(std::vector<RLEmotional*> agents)
{
    for (int i = 0; i < agents.size(); i++)
    {
        agents[i]->reset();
    }
}

double getOverallAverage(std::vector<RLEmotional*> agents)
{
    double average = 0.0;
    for (int i = 0; i < agents.size(); i++)
    {
        average += agents[i]->getAverageScore();
    }
    return average / agents.size();
}

double getMean(std::vector<double> doubles)
{
    double sum = std::accumulate(doubles.begin(), doubles.end(), 0.0);
    return sum/doubles.size();
}

double getDblDiff(double i, double j)
{
    if (i > j)
    {
        return i - j;
    }
    else
    {
        return j - i;
    }
}

double stdDevDouble(std::vector<double> doubles, int size)
{
    double sum = 0.0, mean, standardDeviation = 0.0;
    int i;
    std::vector<double> toSum(doubles.end() - size, doubles.end());
    sum = std::accumulate(toSum.begin(), toSum.end(), 0.0);
    
    mean = sum/toSum.size();
    
    for(i = 0; i < toSum.size(); ++i)
    {
        standardDeviation += pow(toSum[i] - mean, 2);
    }
    
    return sqrt(standardDeviation / toSum.size());
}

double getConfInterval(double z, double SD, double size)
{
    return z * SD / sqrt(size);
}

double getOverallTotal(std::vector<RLEmotional*> agents)
{
    double total = 0.0;
    for (int i = 0; i < agents.size(); i++)
    {
        total += agents[i]->getTotalScore();
    }
    return total;
}


std::map<boost::uuids::uuid, RLEmotional*> assignAgents(std::vector<RLEmotional*> agents, std::set<boost::uuids::uuid> IDs)
{
	std::map<boost::uuids::uuid, RLEmotional*> epucks;
	int i = 0;
	for (std::set<boost::uuids::uuid>::iterator it=IDs.begin(); it!=IDs.end(); ++it)
	{
		epucks[*it] = agents[i];
        i++;
	}
	return epucks;
}

std::vector<RLEmotional*> createAgents(int numOfRobots, StateMode m, bool staghunt, MoodAffectMode mam, double moodAffect)
{
	std::vector<RLEmotional*> agents;
    for (int i = 0; i < numOfRobots; i++)
    {
        agents.push_back(new RLEmotional(m, staghunt, mam, moodAffect));
    }
	random_shuffle(agents.begin(), agents.end());
	return agents;
}
