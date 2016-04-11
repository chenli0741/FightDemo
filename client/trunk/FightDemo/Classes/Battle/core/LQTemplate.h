//
//  LQTemplate.h
//  SuperWarriors
//
//  Created by ByronSong on 13-4-7.
//
//

#ifndef SuperWarriors_LQTemplate_h
#define SuperWarriors_LQTemplate_h

using namespace std;

LQ_NAMESPACE

#pragma mark
#pragma mark container template

template <class T>
void deleteInVector(std::vector<T*>& deleteme) {
    while(!deleteme.empty()) {
        delete deleteme.back();
        deleteme.pop_back();
    }
}

template <class T>
void releaseInVector(std::vector<T*>& deleteme) {
    while(!deleteme.empty()) {
        CC_SAFE_RELEASE( deleteme.back() );
        deleteme.pop_back();
    }
}

template <class T>
void eraseInVector(std::vector<T*>& deleteme, std::string guid) {
    typename std::vector<T*>::iterator it;
    for ( it = deleteme.begin(); it != deleteme.end(); it++)
    {
        if(strcmp(guid.c_str(),(*it)->getGUID().c_str())==0)
            break;
    }
    if(it!=deleteme.end())
        deleteme.erase(it);
}

template <class T1, class T2 >
void deleteInMap(std::map<T1, T2* >& deleteme) {
    typename std::map<T1, T2* >::iterator iter;
    for(iter = deleteme.begin(); iter != deleteme.end(); ++iter) {
        delete iter->second;
    }
    deleteme.clear();
}

template <class T1, class T2 >
void releaseInMap(std::map<T1, T2* >& deleteme) {
    typename std::map<T1, T2* >::iterator iter;
    for(iter = deleteme.begin(); iter != deleteme.end(); ++iter) {
        CC_SAFE_RELEASE( iter->second );
    }
    deleteme.clear();
}

template <class T>
bool isContainsInVector(const std::vector<T>& list, const T& value) {
    typename std::vector<T>::const_iterator iterFind = find(list.begin(), list.end(), value);
    return (iterFind != list.end());
}


template <class T1, class T2 >
void mapToVector(const std::map<T1, T2*>& outMap, std::vector<T2*>& list) {
    typename std::map<T1, T2* >::const_iterator iter;
    for(iter = outMap.begin(); iter != outMap.end(); ++iter) {
        list.push_back(iter->second);
    }
}

#pragma mark
#pragma mark number function


static int maxInt(int value1, int value2){
    return (value1 > value2) ? value1 : value2;
}

static int minInt(int value1, int value2){
    return (value1 > value2) ? value2 : value1;
}

static int absInt(int value){
    return abs(value);
}

static double randomInt(int start, int end, bool issrand = true)
{
    start++;
    end++;
    if (issrand) srand( (unsigned)time( NULL ) );
    return (rand() % (end-start)) + start - 1;
}

static double random(double start, double end)
{
    return start+(end-start)*rand()/(RAND_MAX + 1.0);
}

static double randomWeightInt(int* weight,int size,int w1 = 10)
{
    int max=0;
    for (int i = 0; i<size; i++)
        max += weight[i];   //求权重总值
    int val = randomInt(0, max, false);
    int v1 = 0; int v2 = 0;
    for (int i = 0; i<size; i++){
        v2 += weight[i];
        if (val>=v1 && val<v2){ //命中
            if (weight[i]>w1){  //减少权重逻辑
                for (int j = 0; j<size; j++){
                    if (i!=j && weight[j]>=weight[i]){
                        weight[j] += w1;
                        weight[i] -= w1;
                        if (weight[i]<=w1)
                            break;
                    }
                }
            }
            return i;
        }
        v1 += weight[i];
    }
    
    return -1;
}

LQ_NAMESPACE_END

#endif
