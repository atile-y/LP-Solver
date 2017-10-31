#include "poblacion.h"

#include <QDebug>

#include <random>
#include <chrono>
using namespace std;

Poblacion::Poblacion(){
    m_nSize = 0;
    m_nCromo = 0;
    m_nGenerations = 0;
    m_TipoSeleccion = TipoSeleccion::RULETA;
    m_TipoCruza = TipoCruza::UNPUNTO;
    m_TipoMuta = TipoMuta::BITCHANGE;
    m_nPc = 0.5;
    m_nProbSele = 0.7;
    m_nProbMuta = 0.3;

    m_nPrecision = 2;
    m_bMaximizar = true;

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    generator = default_random_engine(seed);
}

Poblacion::Poblacion(uint s, uint c, TipoSeleccion ts, TipoCruza tc, TipoMuta tm, bool make){
    m_nSize = s;
    m_nCromo = c;
    m_nGenerations = 0;
    m_TipoSeleccion = ts;
    m_TipoCruza = tc;
    m_TipoMuta = tm;
    m_nPc = 0.5;
    m_nProbSele = 0.7;
    m_nProbMuta = 0.3;

    for(uint i=0;i<m_nSize;i++){
        m_vIndividuo.append(new Individuo(m_nCromo));
        if( make )
            m_vIndividuo.at(i)->randomCromosoma();
    }

    m_nPrecision = 2;
    m_bMaximizar = true;

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    generator = default_random_engine(seed);
}

Poblacion::~Poblacion(){
    for(Individuo *ind : m_vIndividuo)
        delete ind;
}

Individuo *Poblacion::getIndividuo(uint i){
    if( i >= m_nSize )
        return NULL;
    return m_vIndividuo.at(i);
}

void Poblacion::setIndividuo(uint i, Individuo *ind){
    if( i >= m_nSize )
        return;

    if( m_vIndividuo.at(i) != NULL )
        delete m_vIndividuo.at(i);

    m_vIndividuo.replace(i, new Individuo(m_nCromo));
    m_vIndividuo.at(i)->setCromosoma(ind->getCromosoma());
}

QVector<double> Poblacion::evolve(){
    QVector<double> data;
    uint idx;
    double max, v;

    calcBits();
    crearG0();
    qInfo() << "Generacion 0:";
    for(uint i=0;i<m_nSize;i++)
        fitness(i, true);


    data.append(getMaximo() / getSuma());
    data.append(getMinimo() / getSuma());

    for(uint i=0;i<m_nGenerations;i++){
        selecciona();
        cruza();
        muta();

        data.append(getMaximo() / getSuma());
        data.append(getMinimo() / getSuma());
        qInfo() << "Generacion" << (i+1);
        idx = 0;
        max = INT_MIN;
        for(uint j=0;j<m_nSize;j++){
            v = fitness(j, true);
            if( v > max ){
                max = v;
                idx = j;
            }
        }

        qInfo() << "Maximo:";
        fitness(idx, true);
        QString strCro = m_vIndividuo.at(idx)->getStrCromosoma();
        int padd = m_vBits.at(0);
        for(int j=1;j<m_vBits.size();j++){
            strCro.insert(padd, "-");
            padd += m_vBits.at(j);
        }
        qInfo() << strCro;
    }

    return data;
}

void Poblacion::calcBits(){
    QPair<bool,bool> reduce;
    m_vBits.clear();
    m_nCromo = 0;
    for(int i=0;i<m_strVar.length();i++){
        long left = m_vLimits.at(i).first;
        long right = m_vLimits.at(i).second;
        long diff = right - left;
        if( std::floor(std::log10(diff)) + m_nPrecision + 1 > 18 )
            reduce = QPair<bool,bool>(left==INT_MIN, right==INT_MAX);
        while( std::floor(std::log10(diff)) + m_nPrecision + 1 > 18 ){
            if( reduce.first )
                left /= 2L;
            if( reduce.second )
                right /= 2L;
            m_vLimits.replace(i, QPair<int,int>(left, right));
            diff = right - left;
        }
        long mult = std::pow(10, m_nPrecision);
        m_vBits.append(std::ceil(std::log2(diff*mult)));
        m_nCromo += m_vBits.at(i);
        qInfo() << "Variable" << m_strVar.at(i) << " => " << m_vBits.at(i) << "bits.";
    }
    qInfo() << "Cromosoma" << m_nCromo << "bits.";
}

void Poblacion::crearG0(){
    m_vIndividuo.clear();

    for(uint i=0;i<m_nSize;i++){
        m_vIndividuo.append(new Individuo(m_nCromo));
        do{
            m_vIndividuo.at(i)->randomCromosoma();
        }
        while( !valida(i) );
    }
}

bool Poblacion::valida(uint idx){
    QHash<QString, double> hash;
    uint bitini = 0;
    for(int i=0;i<m_strVar.length();i++){
        ulong decimal = m_vIndividuo.at(idx)->getDecimal(bitini, bitini + m_vBits.at(i));
        double diff = m_vLimits.at(i).second - m_vLimits.at(i).first;
        hash[m_strVar.at(i)] = m_vLimits.at(i).first + decimal * (diff / (1L << m_vBits.at(i)));
        bitini += m_vBits.at(i);
    }

    //qInfo() << hash;

    QRegExp reNum("-?(?:\\d*\\.)?\\d+");
    QRegExp reVar("[a-y]", Qt::CaseInsensitive);
    QRegExp reDesig("<|<=|>|>=|=");
    for(int i=0;i<m_vR.size();i++){
        QString comp;
        double left = 0.0, right = 0.0;
        double tmp = 1.0;
        int j = 0;

        if( reNum.indexIn(m_vR.at(i).at(j)) != -1 )
            left = reNum.cap().toDouble();
        else
            left = 1.0;
        if( reVar.indexIn(m_vR.at(i).at(j)) != -1 )
            left *= hash[reVar.cap()];
        for(j=1;j<m_vR.at(i).size();j++){
            if( reDesig.indexIn(m_vR.at(i).at(j)) != -1 ){
                comp = reDesig.cap();
                j++;
                break;
            }

            if( m_vR.at(i).at(j) == "+" )
                tmp = 1.0;
            else if( m_vR.at(i).at(j) == "-" )
                tmp = -1.0;
            else
                qInfo() << "FATAL ERROR";

            j++;
            if( reNum.indexIn(m_vR.at(i).at(j)) != -1 )
                tmp *= reNum.cap().toDouble();
            if( reVar.indexIn(m_vR.at(i).at(j)) != -1 )
                tmp *= hash[reVar.cap()];

            left += tmp;
        }

        if( reNum.indexIn(m_vR.at(i).at(j)) != -1 )
            right = reNum.cap().toDouble();
        else
            right = 1.0;
        if( reVar.indexIn(m_vR.at(i).at(j)) != -1 )
            right *= hash[reVar.cap()];
        for(j++;j<m_vR.at(i).size();j++){
            if( m_vR.at(i).at(j) == "+" )
                tmp = 1.0;
            else if( m_vR.at(i).at(j) == "-" )
                tmp = -1.0;
            else
                qInfo() << "FATAL ERROR";

            j++;
            if( reNum.indexIn(m_vR.at(i).at(j)) != -1 )
                tmp *= reNum.cap().toDouble();
            if( reVar.indexIn(m_vR.at(i).at(j)) != -1 )
                tmp *= hash[reVar.cap()];

            right += tmp;
        }

        //qInfo() << left << comp << right;

        if( comp == "<" && !(left < right) )
            return false;
        else if( comp == "<=" && !(left <= right) )
            return false;
        else if( comp == ">" && !(left > right) )
            return false;
        else if( comp == ">=" && !(left >= right) )
            return false;
        else if( comp == "=" && !(left == right) )
            return false;
    }

    return true;
}

double Poblacion::getSuma(){
    double r = 0;

    for(uint i=0;i<m_nSize;i++)
        r += fitness(i);

    return r;
}

double Poblacion::getMaximo(){
    double r = fitness(0), v;

    for(uint i=1;i<m_nSize;i++){
        v = fitness(i);
        if( v > r )
            r = v;
    }

    return r;
}

double Poblacion::getMinimo(){
    double r = fitness(0), v;

    for(uint i=1;i<m_nSize;i++){
        v = fitness(i);
        if( v < r )
            r = v;
    }

    return r;
}

double Poblacion::fitness(uint idx, bool debug){
    if( idx >= m_nSize )
        return -999999.99;

    QString strDebug = "";
    QHash<QString, double> hash;
    uint bitini = 0;
    for(int i=0;i<m_strVar.length();i++){
        ulong decimal = m_vIndividuo.at(idx)->getDecimal(bitini, bitini + m_vBits.at(i));
        double diff = m_vLimits.at(i).second - m_vLimits.at(i).first;
        hash[m_strVar.at(i)] = m_vLimits.at(i).first + decimal * (diff / (1L << m_vBits.at(i)));
        if( debug )
            strDebug += QString(m_strVar.at(i)) + "=" + QString::number(hash[m_strVar.at(i)]) + ",  ";
        bitini += m_vBits.at(i);
    }

    QRegExp reNum("-?(?:\\d*\\.)?\\d+");
    QRegExp reVar("[a-y]", Qt::CaseInsensitive);
    double res = 0;
    double tmp = 1.0;
    int j = 0;

    if( reNum.indexIn(m_slZ.at(j)) != -1 )
        res = reNum.cap().toDouble();
    else
        res = 1.0;
    if( reVar.indexIn(m_slZ.at(j)) != -1 )
        res *= hash[reVar.cap()];
    for(j=1;j<m_slZ.size();j++){
        if( m_slZ.at(j) == "+" )
            tmp = 1.0;
        else if( m_slZ.at(j) == "-" )
            tmp = -1.0;
        else
            qInfo() << "FATAL ERROR";

        j++;
        if( reNum.indexIn(m_slZ.at(j)) != -1 )
            tmp *= reNum.cap().toDouble();
        if( reVar.indexIn(m_slZ.at(j)) != -1 )
            tmp *= hash[reVar.cap()];

        res += tmp;
    }

    if( debug ){
        strDebug += "Z=" + QString::number(res);
        qInfo() << strDebug;
    }
    return res;
}

QVector<Individuo *> Poblacion::cruza(uint i, uint j){
    if( i >= m_nSize || j >= m_nSize )
        return QVector<Individuo *>();

    Individuo *a = m_vIndividuo[i];
    Individuo *b = m_vIndividuo[j];

    if( a->getTam() != m_nCromo || b->getTam() != m_nCromo )
        return QVector<Individuo *>();

    uint p1, p2, tmp, i1, i2, len, pos;
    uint *h1 = new uint[m_nCromo];
    uint *h2 = new uint[m_nCromo];
    bool *pat1 = new bool[m_nCromo];
    bool *pat2 = new bool[m_nCromo];
    uint *data;
    bool tmp2;
    QVector<Individuo *> v;
    QVector<uint> cycle;
    Individuo *m1, *m2;
    uniform_int_distribution<uint> dist, d2;
    uniform_real_distribution<double> d;

    switch(m_TipoCruza){
    case UNPUNTO:
        dist = uniform_int_distribution<uint>(1, m_nCromo-1);
        p1 = dist(generator);

        memcpy(h1, a->getCromosoma(), p1*sizeof(h1[0]));
        memcpy(h2, b->getCromosoma(), p1*sizeof(h1[0]));

        memcpy(h1+p1, b->getCromosoma()+p1, (m_nCromo-p1)*sizeof(h1[0]));
        memcpy(h2+p1, a->getCromosoma()+p1, (m_nCromo-p1)*sizeof(h1[0]));

        break;

    case DOSPUNTOS:
        dist = uniform_int_distribution<uint>(1, m_nCromo-1);
        p1 = dist(generator);
        do{
            p2 = dist(generator);
        }
        while( p2 == p1 );

        if( p2 < p1 ){
            tmp = p1;
            p1 = p2;
            p2 = tmp;
        }

        memcpy(h1, a->getCromosoma(), p1*sizeof(h1[0]));
        memcpy(h2, b->getCromosoma(), p1*sizeof(h1[0]));

        memcpy(h1+p1, b->getCromosoma()+p1, (p2-p1)*sizeof(h1[0]));
        memcpy(h2+p1, a->getCromosoma()+p1, (p2-p1)*sizeof(h1[0]));

        memcpy(h1+p2, a->getCromosoma()+p2, (m_nCromo-p2)*sizeof(h1[0]));
        memcpy(h2+p2, b->getCromosoma()+p2, (m_nCromo-p2)*sizeof(h1[0]));

        break;

    case UNIFORME:
        d = uniform_real_distribution<double>(0.0, 1.0);
        for(uint k=0;k<m_nCromo;k++){
            if( d(generator) < m_nPc ){
                h1[k] = a->getAlelo(k);
                h2[k] = b->getAlelo(k);
                pat1[k] = true;
            }
            else{
                h1[k] = b->getAlelo(k);
                h2[k] = a->getAlelo(k);
                pat1[k] = false;
            }
        }

        break;

    case ACENTUADA:
        memcpy(pat1, a->getPattern(), m_nCromo);
        memcpy(pat2, b->getPattern(), m_nCromo);
        tmp = 0;
        for(uint k=0;k<m_nCromo;k++){
            if( tmp == 0 ){
                h1[k] = a->getCromosoma()[k];
                h2[k] = b->getCromosoma()[k];
                if( pat1[k] || pat2[k] )
                    tmp = 1;
            }
            else if( tmp == 1 ){
                h1[k] = b->getCromosoma()[k];
                h2[k] = a->getCromosoma()[k];
                if( pat1[k] || pat2[k] ){
                    tmp2 = pat1[k];
                    pat1[k] = pat2[k];
                    pat2[k] = tmp2;
                    tmp = 0;
                }
            }
        }

        break;

    case OX:
        data = new uint[2];
        dist = uniform_int_distribution<uint>(0, m_nCromo-3);
        data[0] = pos = dist(generator);
        d2 = uniform_int_distribution<uint>(2, m_nCromo-pos-1);
        data[1] = len = d2(generator);

        m1 = new Individuo(2);
        m1->setCromosoma(data);
        v.push_back(m1);
        i1=0;
        i2=0;
        for(uint k=0;k<pos;k++){
            do{
                tmp2 = false;
                for(uint ii=0;ii<len && !tmp2;ii++)
                    if( b->getAlelo(i2) == a->getAlelo(pos+ii) )
                        tmp2 = true;
                i2++;
            }
            while( tmp2 );
            h1[k] = b->getAlelo(i2-1);

            do{
                tmp2 = false;
                for(uint ii=0;ii<len && !tmp2;ii++)
                    if( a->getAlelo(i1) == b->getAlelo(pos+ii) )
                        tmp2 = true;
                i1++;
            }
            while( tmp2 );
            h2[k] = a->getAlelo(i1-1);
        }

        memcpy(h1+pos, (a->getCromosoma())+pos, len*sizeof(uint));
        memcpy(h2+pos, (b->getCromosoma())+pos, len*sizeof(uint));

        for(uint k=pos+len;k<m_nCromo;k++){
            do{
                tmp2 = false;
                for(uint ii=0;ii<len && !tmp2;ii++)
                    if( b->getAlelo(i2) == a->getAlelo(pos+ii) )
                        tmp2 = true;
                i2++;
            }
            while( tmp2 );
            h1[k] = b->getAlelo(i2-1);

            do{
                tmp2 = false;
                for(uint ii=0;ii<len && !tmp2;ii++)
                    if( a->getAlelo(i1) == b->getAlelo(pos+ii) )
                        tmp2 = true;
                i1++;
            }
            while( tmp2 );
            h2[k] = a->getAlelo(i1-1);
        }
        break;

    case PMX:
        data = new uint[2];
        dist = uniform_int_distribution<uint>(1, m_nCromo-1);
        data[0] = p1 = dist(generator);
        do{
            data[1] = p2 = dist(generator);
        }
        while( p1 == p2 );
        if( p2 < p1 ){
            tmp = p2;
            data[1] = p2 = p1;
            data[0] = p1 = tmp;
        }

        m1 = new Individuo(2);
        m1->setCromosoma(data);
        v.push_back(m1);
        for(uint k=0;k<p1;k++){
            tmp2 = false;
            for(tmp=p1;tmp<p2 && !tmp2;tmp++)
                if( b->getAlelo(tmp) == a->getAlelo(k) )
                    tmp2 = true;
            if( tmp2 ){
                tmp--;
                do{
                    tmp = a->getAlelo(tmp);
                    tmp2 = false;
                    for(uint ii=p1;ii<p2 && !tmp2;ii++)
                        if( b->getAlelo(ii) == tmp ){
                            tmp2 = true;
                            tmp = ii;
                        }
                }
                while( tmp2 );
                h1[k] = tmp;
            }
            else
                h1[k] = a->getAlelo(k);

            tmp2 = false;
            for(tmp=p1;tmp<p2 && !tmp2;tmp++)
                if( a->getAlelo(tmp) == b->getAlelo(k) )
                    tmp2 = true;
            if( tmp2 ){
                tmp--;
                do{
                    tmp = b->getAlelo(tmp);
                    tmp2 = false;
                    for(uint ii=p1;ii<p2 && !tmp2;ii++)
                        if( a->getAlelo(ii) == tmp ){
                            tmp2 = true;
                            tmp = ii;
                        }
                }
                while( tmp2 );
                h2[k] = tmp;
            }
            else
                h2[k] = b->getAlelo(k);
        }

        memcpy(h1+p1, (b->getCromosoma())+p1, (p2-p1)*sizeof(uint));
        memcpy(h2+p1, (a->getCromosoma())+p1, (p2-p1)*sizeof(uint));

        for(uint k=p2;k<m_nCromo;k++){
            tmp2 = false;
            for(tmp=p1;tmp<p2 && !tmp2;tmp++)
                if( b->getAlelo(tmp) == a->getAlelo(k) )
                    tmp2 = true;
            if( tmp2 ){
                tmp--;
                do{
                    tmp = a->getAlelo(tmp);
                    tmp2 = false;
                    for(uint ii=p1;ii<p2 && !tmp2;ii++)
                        if( b->getAlelo(ii) == tmp ){
                            tmp2 = true;
                            tmp = ii;
                        }
                }
                while( tmp2 );
                h1[k] = tmp;
            }
            else
                h1[k] = a->getAlelo(k);

            tmp2 = false;
            for(tmp=p1;tmp<p2 && !tmp2;tmp++)
                if( a->getAlelo(tmp) == b->getAlelo(k) )
                    tmp2 = true;
            if( tmp2 ){
                tmp--;
                do{
                    tmp = b->getAlelo(tmp);
                    tmp2 = false;
                    for(uint ii=p1;ii<p2 && !tmp2;ii++)
                        if( a->getAlelo(ii) == tmp ){
                            tmp2 = true;
                            tmp = ii;
                        }
                }
                while( tmp2 );
                h2[k] = tmp;
            }
            else
                h2[k] = b->getAlelo(k);
        }
        break;

    case PBC:
        dist = uniform_int_distribution<uint>(2, m_nCromo-2);
        len = dist(generator);
        data = new uint[len];
        memset(data, 0, len*sizeof(data[0]));
        d2 = uniform_int_distribution<uint>(1, m_nCromo);
        for(uint ii=0;ii<len;ii++){
            do{
                tmp = d2(generator);
                tmp2 = false;
                for(uint jj=0;jj<len && !tmp2;jj++)
                    if( data[jj] == tmp )
                        tmp2 = true;
            }
            while( tmp2 );
            data[ii] = tmp;
        }

        m1 = new Individuo(len);
        m1->setCromosoma(data);
        v.push_back(m1);

        data = new uint[len];
        memset(data, 0, len*sizeof(data[0]));
        for(uint ii=0;ii<len;ii++){
            do{
                tmp = d2(generator);
                tmp2 = false;
                for(uint jj=0;jj<len && !tmp2;jj++)
                    if( data[jj] == tmp )
                        tmp2 = true;
            }
            while( tmp2 );
            data[ii] = tmp;
        }

        m2 = new Individuo(len);
        m2->setCromosoma(data);
        v.push_back(m2);

        i1=0;
        i2=0;
        for(uint k=0;k<m_nCromo;k++){
            tmp2 = false;
            for(uint ii=0;ii<len && !tmp2;ii++)
                if( a->getAlelo(k) == m1->getAlelo(ii) )
                    tmp2 = true;
            if( !tmp2 ){
                do{
                    tmp2 = false;
                    for(uint ii=0;ii<len && !tmp2;ii++)
                        if( b->getAlelo(i2) == m1->getAlelo(ii) )
                            tmp2 = true;
                    i2++;
                }
                while( tmp2 );
                h1[k] = b->getAlelo(i2-1);
            }
            else
                h1[k] = a->getAlelo(k);

            tmp2 = false;
            for(uint ii=0;ii<len && !tmp2;ii++)
                if( b->getAlelo(k) == m2->getAlelo(ii) )
                    tmp2 = true;
            if( !tmp2 ){
                do{
                    tmp2 = false;
                    for(uint ii=0;ii<len && !tmp2;ii++)
                        if( a->getAlelo(i1) == m2->getAlelo(ii) )
                            tmp2 = true;
                    i1++;
                }
                while( tmp2 );
                h2[k] = a->getAlelo(i1-1);
            }
            else
                h2[k] = b->getAlelo(k);
        }
        break;

    case OBC:
        dist = uniform_int_distribution<uint>(2, m_nCromo-2);
        len = dist(generator);
        data = new uint[len];
        memset(data, 0, len*sizeof(data[0]));
        d2 = uniform_int_distribution<uint>(1, m_nCromo);
        for(uint ii=0;ii<len;ii++){
            do{
                tmp = d2(generator);
                tmp2 = false;
                for(uint jj=0;jj<len && !tmp2;jj++)
                    if( data[jj] == tmp )
                        tmp2 = true;
            }
            while( tmp2 );
            data[ii] = tmp;
        }
        qsort(data, len, sizeof(data[0]), [](const void *a, const void *b){
            return *(int *)a - *(int *)b;
        });
        for(uint ii=0;ii<len;ii++)
            data[ii] = a->getAlelo(data[ii]-1);

        m1 = new Individuo(len);
        m1->setCromosoma(data);
        v.push_back(m1);

        data = new uint[len];
        memset(data, 0, len*sizeof(data[0]));
        for(uint ii=0;ii<len;ii++){
            do{
                tmp = d2(generator);
                tmp2 = false;
                for(uint jj=0;jj<len && !tmp2;jj++)
                    if( data[jj] == tmp )
                        tmp2 = true;
            }
            while( tmp2 );
            data[ii] = tmp;
        }
        qsort(data, len, sizeof(data[0]), [](const void *a, const void *b){
            return *(int *)a - *(int *)b;
        });
        for(uint ii=0;ii<len;ii++)
            data[ii] = b->getAlelo(data[ii]-1);

        m2 = new Individuo(len);
        m2->setCromosoma(data);
        v.push_back(m2);

        i1=0;
        i2=0;
        for(uint k=0;k<m_nCromo;k++){
            tmp2 = false;
            for(uint ii=0;ii<len && !tmp2;ii++)
                if( b->getAlelo(k) == m1->getAlelo(ii) )
                    tmp2 = true;
            if( tmp2 )
                h1[k] = m1->getAlelo(i1++);
            else
                h1[k] = b->getAlelo(k);

            tmp2 = false;
            for(uint ii=0;ii<len && !tmp2;ii++)
                if( a->getAlelo(k) == m2->getAlelo(ii) )
                    tmp2 = true;
            if( tmp2 )
                h2[k] = m2->getAlelo(i2++);
            else
                h2[k] = a->getAlelo(k);
        }
        break;

    case CX:
        dist = uniform_int_distribution<uint>(0, m_nCromo-1);
        pos = dist(generator);

        cycle.clear();
        cycle.append(a->getAlelo(pos));
        if( cycle[0] != b->getAlelo(pos) )
            cycle.append(b->getAlelo(pos));

        tmp2 = cycle.size() == 2;
        tmp = b->getAlelo(pos);
        while(tmp2){
            for(i1=0;i1<m_nCromo;i1++)
                if( a->getAlelo(i1) == tmp )
                    break;
            tmp2 = b->getAlelo(i1) != cycle[0];
            if( tmp2 ){
                tmp = b->getAlelo(i1);
                cycle.append(tmp);
            }
        }

        len = cycle.size();
        data = new uint[len];
        for(uint ii=0;ii<len;ii++)
            data[ii] = cycle[ii];

        m1 = new Individuo(len);
        m1->setCromosoma(data);
        v.push_back(m1);

        i1=0;
        i2=0;
        for(uint k=0;k<m_nCromo;k++){
            tmp2 = false;
            for(uint ii=0;ii<len && !tmp2;ii++)
                if( a->getAlelo(k) == m1->getAlelo(ii) )
                    tmp2 = true;
            if( !tmp2 ){
                do{
                    tmp2 = false;
                    for(uint ii=0;ii<len && !tmp2;ii++)
                        if( b->getAlelo(i2) == m1->getAlelo(ii) )
                            tmp2 = true;
                    i2++;
                }
                while( tmp2 );
                h1[k] = b->getAlelo(i2-1);
            }
            else
                h1[k] = a->getAlelo(k);

            tmp2 = false;
            for(uint ii=0;ii<len && !tmp2;ii++)
                if( b->getAlelo(k) == m1->getAlelo(ii) )
                    tmp2 = true;
            if( !tmp2 ){
                do{
                    tmp2 = false;
                    for(uint ii=0;ii<len && !tmp2;ii++)
                        if( a->getAlelo(i1) == m1->getAlelo(ii) )
                            tmp2 = true;
                    i1++;
                }
                while( tmp2 );
                h2[k] = a->getAlelo(i1-1);
            }
            else
                h2[k] = b->getAlelo(k);
        }
        break;

    default:
        return QVector<Individuo *>();
    }

    Individuo *ind = new Individuo(m_nCromo);
    ind->setCromosoma(h2);
    v.push_front(ind);

    ind = new Individuo(m_nCromo);
    ind->setCromosoma(h1);
    v.push_front(ind);

    return v;
}

Individuo *Poblacion::muta(uint i){
    if( i >= m_nSize )
        return NULL;

    uniform_real_distribution<double> prob(0.0, 1.0);
    if( prob(generator) > m_nProbMuta )
        return NULL;

    Individuo *a = m_vIndividuo.at(i);

    uint p1, p2, tmp, len, val;
    uint *h1 = new uint[m_nCromo];
    uint *h2 = new uint[m_nCromo];
    uint *idx;
    bool flag;
    Individuo *m1, *m2;
    uniform_int_distribution<uint> dist, d2;

    switch(m_TipoMuta){
    case TipoMuta::BITCHANGE:
        dist = uniform_int_distribution<uint>(0, m_nCromo-1);
        p1 = dist(generator);
        memcpy(h1, a->getCromosoma(), m_nCromo*sizeof(h1[0]));
        h1[p1] = h1[p1]?0:1;
        m1 = new Individuo(m_nCromo);
        m1->setCromosoma(h1);

        return m1;

    case TipoMuta::INSER:
        dist = uniform_int_distribution<uint>(0, m_nCromo-1);
        p1 = dist(generator);
        do{ p2 = dist(generator); }
        while( p1 == p2 );

        if( p1 < p2 ){
            memcpy(h1, a->getCromosoma(), p1*sizeof(h1[0]));
            memcpy(h1+p1, a->getCromosoma()+p1+1, (p2-p1)*sizeof(h1[0]));
            h1[p2] = a->getAlelo(p1);
            memcpy(h1+p2+1, a->getCromosoma()+p2+1, (m_nCromo-p2-1)*sizeof(h1[0]));
        }
        else{
            memcpy(h1, a->getCromosoma(), p2*sizeof(h1[0]));
            h1[p2] = a->getAlelo(p1);
            memcpy(h1+p2+1, a->getCromosoma()+p2, (p1-p2)*sizeof(h1[0]));
            memcpy(h1+p1+1, a->getCromosoma()+p1+1, (m_nCromo-p1-1)*sizeof(h1[0]));
        }
        m1 = new Individuo(m_nCromo);
        m1->setCromosoma(h1);
        return m1;

    case TipoMuta::DESPLA:
        d2 = uniform_int_distribution<uint>(2, 6);
        dist = uniform_int_distribution<uint>(0, m_nCromo-1);
        len = d2(generator);
        memcpy(h2, a->getCromosoma(), m_nCromo*sizeof(h2[0]));
        for(uint i=0;i<len;i++){
            p1 = dist(generator);
            do{ p2 = dist(generator); }
            while( p1 == p2 );

            if( p1 < p2 ){
                memcpy(h1, h2, p1*sizeof(h1[0]));
                memcpy(h1+p1, h2+p1+1, (p2-p1)*sizeof(h1[0]));
                h1[p2] = h2[p1];
                memcpy(h1+p2+1, h2+p2+1, (m_nCromo-p2-1)*sizeof(h1[0]));
            }
            else{
                memcpy(h1, h2, p2*sizeof(h1[0]));
                h1[p2] = h2[p1];
                memcpy(h1+p2+1, h2+p2, (p1-p2)*sizeof(h1[0]));
                memcpy(h1+p1+1, h2+p1+1, (m_nCromo-p1-1)*sizeof(h1[0]));
            }

            memcpy(h2, h1, m_nCromo*sizeof(h2[0]));
        }
        m1 = new Individuo(m_nCromo);
        m1->setCromosoma(h1);
        return m1;

    case TipoMuta::INTER_RECIP:
        dist = uniform_int_distribution<uint>(0, m_nCromo-1);
        p1 = dist(generator);
        do{ p2 = dist(generator); }
        while( p1 == p2 );

        memcpy(h1, a->getCromosoma(), m_nCromo*sizeof(h1[0]));

        tmp = h1[p1];
        h1[p1] = h1[p2];
        h1[p2] = tmp;

        m1 = new Individuo(m_nCromo);
        m1->setCromosoma(h1);
        return m1;

    case TipoMuta::HEURIS:
        d2 = uniform_int_distribution<uint>(2, (7<m_nCromo-1?7:m_nCromo-1));
        dist = uniform_int_distribution<uint>(0, m_nCromo-1);
        p1 = d2(generator);
        idx = new uint[p1];
        for(uint i=0;i<p1;i++){
            do{
                idx[i] = dist(generator);
                flag = false;
                for(uint j=0;j<i && !flag;j++)
                    if( idx[j] == idx[i] )
                        flag = true;
            }
            while( flag );
        }
        sort(idx, idx+p1);

        h1 = new uint[p1];
        memcpy(h2, a->getCromosoma(), m_nCromo*sizeof(h2[0]));
        for(uint i=0;i<p1;i++)
            h1[i] = idx[i];

        m1 = new Individuo(m_nCromo);
        m2 = new Individuo(m_nCromo);
        val = 0;
        while( next_permutation(h1, h1+p1) ){
            for(uint i=0;i<p1;i++)
                h2[idx[i]] = a->getAlelo(h1[i]);
            // Funcion de Aptitud
            m2->setCromosoma(h2);
            if( h1[0]*p1 > val ){ // m2->getFitness()
                val = h1[0]*p1;
                m1->setCromosoma(h2);
            }
        }

        return m1;

    default:
        return NULL;
    }
}

void Poblacion::selecciona(){
    QVector<Individuo *> pob2;
    QVector<uint> pobIdx;
    QVector<double> acum;
    uniform_real_distribution<double> prob;
    uint idxO, idxB;
    double num, left, right;
    bool isGT;

    switch(m_TipoSeleccion){
    case TipoSeleccion::TORNEO:
        prob = uniform_real_distribution<double>(0.0, 1.0);
        for(int p=0;p<2;p++){
            pobIdx = barajea();
            for(uint i=0;i<m_nSize;i+=2){
                left = fitness(pobIdx.at(i));
                right = fitness(pobIdx.at(i+1));
                isGT = left > right;
                if( !(isGT ^ m_bMaximizar) ){
                    idxO = pobIdx.at(i);
                    idxB = pobIdx.at(i+1);
                }
                else{
                    idxO = pobIdx.at(i+1);
                    idxB = pobIdx.at(i);
                }
                if( prob(generator) <= m_nProbSele )
                    pob2.append(m_vIndividuo.at(idxO));
                else
                    pob2.append(m_vIndividuo.at(idxB));
            }
        }
        break;

    case TipoSeleccion::RULETA:
        prob = uniform_real_distribution<double>(0.0, 1.0);
        double suma = getSuma();
        acum.append(fitness(0)/suma);
        for(uint i=1;i<m_nSize;i++)
            acum.append(acum.at(i-1) + fitness(i)/suma);

        for(uint i=0;i<m_nSize;i++){
            num = prob(generator);
            for(uint j=0;j<m_nSize;j++){
                if( num < acum.at(j) ){
                    pob2.append(m_vIndividuo.at(j));
                    break;
                }
            }
        }
        break;
    }

    for(uint i=0;i<m_nSize;i++)
        m_vIndividuo.replace(i, pob2.at(i));
}

void Poblacion::cruza(){
    QVector<Individuo *> v;

    for(uint i=0;i<m_nSize;i+=2){
        v = cruza(i, i+1);

        m_vIndividuo.replace(i, v.at(0));
        m_vIndividuo.replace(i+1, v.at(1));
        while( !valida(i) )
            m_vIndividuo.at(i)->randomCromosoma();
        while( !valida(i+1) )
            m_vIndividuo.at(i+1)->randomCromosoma();
    }
}

void Poblacion::muta(){
    int numMuta = m_nProbMuta * m_nSize;
    uniform_real_distribution<double> p(0.0, 1.0);
    if( p(generator) <= m_nProbMuta )
        numMuta++;

    QVector<uint>v;
    uniform_int_distribution<uint> dist(0, m_nSize-1);
    uint r;

    for(int i=0;i<numMuta;i++){
        do{
            r = dist(generator);
        }
        while( v.contains(r) );
        v.append(r);
    }

    Individuo *ind;
    for(uint i : v){
        ind = muta(i);
        if( ind != NULL ){
            m_vIndividuo.replace(i, ind);
            while( !valida(i) )
                m_vIndividuo.at(i)->randomCromosoma();
        }
    }
}

QVector<uint> Poblacion::barajea(){
    QVector<uint> idx;
    uniform_int_distribution<uint> dist(0, m_nSize-1);
    uint n;

    for(uint i=0;i<m_nSize;i++){
        do{
            n = dist(generator);
        }
        while( idx.indexOf(n) != -1 );
        idx.append(n);
    }

    return idx;
}
