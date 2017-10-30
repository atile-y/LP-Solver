#ifndef INDIVIDUO_H
#define INDIVIDUO_H

#include <QString>

#include <random>

class Individuo{
public:
    Individuo(uint);
   ~Individuo();

    uint getTam(){ return m_nTam; }
    uint *getCromosoma(){ return m_bCromosoma; }
    uint getAlelo(uint);
    bool *getPattern(){ return m_bPattern; }

    void setTam(uint);
    void setCromosoma(uint *);
    void setPattern(bool *);

    void randomCromosoma();
    void randomPattern();
    QString getStrCromosoma();
    uint getDecimal();

private:
    uint m_nTam;
    uint *m_bCromosoma;
    bool *m_bPattern;
    std::default_random_engine generator;
};

#endif // INDIVIDUO_H
