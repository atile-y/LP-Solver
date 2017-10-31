#include "individuo.h"

#include <cstring>

#include <chrono>
using namespace std;

Individuo::Individuo(uint t){
    m_nTam = t;
    m_bCromosoma = new uint[t];
    m_bPattern = NULL;

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    generator = default_random_engine(seed);
}

Individuo::~Individuo(){
    delete m_bCromosoma;
    if( m_bPattern != NULL )
        delete m_bPattern;
}

uint Individuo::getAlelo(uint i){
    if( i >= m_nTam )
        return false;
    return m_bCromosoma[i];
}

void Individuo::setTam(uint t){
    m_nTam = t;
    delete m_bCromosoma;
    m_bCromosoma = new uint[m_nTam];
}

void Individuo::setCromosoma(uint *c){
    memcpy(m_bCromosoma, c, m_nTam * sizeof(uint));
}

void Individuo::setPattern(bool *p){
    if( m_bPattern == NULL )
        m_bPattern = new bool[m_nTam];
    memcpy(m_bPattern, p, m_nTam * sizeof(bool));
}

void Individuo::randomCromosoma(){
/*
    uniform_int_distribution<uint> dist(0, m_nTam-1);
    uint n;

    for(uint i=0;i<m_nTam;i++)
        m_bCromosoma[i] = 0;

    for(uint i=0;i<m_nTam;i++){
        do{
            n = dist(generator);
        }
        while( m_bCromosoma[n] != 0 );
        m_bCromosoma[n] = i+1;
    }
*/
    uniform_int_distribution<uint> dist;
    uint n;

    for(uint i=0;i<m_nTam;){
        n = dist(generator);
        for(int j=0;j<32 && i<m_nTam;j++,i++){
            m_bCromosoma[i] = n%2 == 1;
            n /= 2;
        }
    }
}

void Individuo::randomPattern(){
    uniform_int_distribution<int> dist(0, m_nTam-2);

    if( m_bPattern == NULL )
        m_bPattern = new bool[m_nTam];
    memset(m_bPattern, 0, m_nTam);
    for(uint i=0;i<3;i++)
        m_bPattern[dist(generator)] = true;
}

QString Individuo::getStrCromosoma(){
    QString ret = "";

    for(uint i=0;i<m_nTam;i++){
        ret += QString::number(m_bCromosoma[i]);
    }

    return ret;
}

uint Individuo::getDecimal(){
    uint v = 0;
    for(uint i=0;i<m_nTam;i++)
        v = 2*v + m_bCromosoma[i];
    return v;
}

ulong Individuo::getDecimal(uint ini, uint fin){
    ulong v = 0L;
    for(uint i=ini;i<fin;i++)
        v = 2L*v + m_bCromosoma[i];
    return v;
}
