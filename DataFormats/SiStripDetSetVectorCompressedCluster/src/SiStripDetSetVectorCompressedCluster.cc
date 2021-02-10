#include "DataFormats/SiStripDetSetVectorCompressedCluster/interface/SiStripDetSetVectorCompressedCluster.h"

SiStripDetSetVectorCompressedCluster::SiStripDetSetVectorCompressedCluster(){
    compressedAmplitudes_.clear();
    firstStrip_.clear();
    //error_x_.clear();
}

SiStripDetSetVectorCompressedCluster::SiStripDetSetVectorCompressedCluster(std::vector<std::pair<uint16_t, bool>>& firstStripMerged, std::vector<float>& errx,std::vector<uint8_t>& inVect){
    compressedAmplitudes_.clear();
    firstStrip_.clear();
   // error_x_.clear();

    for(auto itFs : firstStripMerged ) this->push_back_firstStip(itFs.first, itFs.second);
    for(auto itErr: errx) this->push_back_splitClusterError(itErr);
    this->loadCompressedAmplitudes(inVect);
}

void SiStripDetSetVectorCompressedCluster::push_back_supportInfo(uint16_t firstStrip, bool merged, float errx){
    this->push_back_firstStip(firstStrip, merged);
    this->push_back_splitClusterError(errx);
}

void SiStripDetSetVectorCompressedCluster::loadCompressedAmplitudes(std::vector<uint8_t>& inVect){
    compressedAmplitudes_.clear();
    compressedAmplitudes_.assign(inVect.begin(), inVect.end());
}

void SiStripDetSetVectorCompressedCluster::addCompressedAmplitudes(std::vector<uint8_t>& inVect){
    compressedAmplitudes_.insert(compressedAmplitudes_.end(), inVect.begin(), inVect.end());
}