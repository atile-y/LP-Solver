#ifndef POBLACION_H
#define POBLACION_H

#include "individuo.h"

#include <QVector>

#include <random>

enum TipoSeleccion{ TORNEO, RULETA };
enum TipoCruza{ UNPUNTO, DOSPUNTOS, UNIFORME, ACENTUADA, OX, PMX, PBC, OBC, CX };
enum TipoMuta{ BITCHANGE, INSER, DESPLA, INTER_RECIP, HEURIS };

class Poblacion{
public:
    Poblacion();
    Poblacion(uint, uint, TipoSeleccion = TipoSeleccion::RULETA, TipoCruza = TipoCruza::UNPUNTO, TipoMuta = TipoMuta::BITCHANGE, bool = false);
    ~Poblacion();

    uint getSize(){ return m_nSize; }
    uint getCromo(){ return m_nCromo; }
    uint getGenerations(){ return m_nGenerations; }
    TipoSeleccion getTipoSeleccion(){ return m_TipoSeleccion; }
    TipoCruza getTipoCruza(){ return m_TipoCruza; }
    TipoMuta getTipoMuta(){ return m_TipoMuta; }
    double getPC(){ return m_nPc; }
    double getProbSele(){ return m_nProbSele; }
    double getProbMuta(){ return m_nProbMuta; }
    Individuo *getIndividuo(uint);

    void setSize(uint s){ m_nSize = s; }
    void setCromo(uint c){ m_nCromo = c; }
    void setGenerations(uint g){ m_nGenerations = g; }
    void setTipoSeleccion(TipoSeleccion t){ m_TipoSeleccion = t; }
    void setTipoCruza(TipoCruza t){ m_TipoCruza = t; }
    void setTipoMuta(TipoMuta t){ m_TipoMuta = t; }
    void setPC(double pc){ m_nPc = pc; }
    void setProbSele(double ps){ m_nProbSele = ps; }
    void setProbMuta(double pm){ m_nProbMuta = pm; }
    void setIndividuo(uint, Individuo *);

    QVector<double> evolve();

    void crearG0();
    double getSuma();
    double getMaximo();
    double getMinimo();

    double fitness(uint);

    QVector<Individuo *> cruza(uint, uint);
    Individuo *muta(uint);

    void selecciona();
    void cruza();
    void muta();

private:
    QVector<uint> barajea();

    uint m_nSize;
    uint m_nCromo;
    uint m_nGenerations;
    TipoSeleccion m_TipoSeleccion;
    TipoCruza m_TipoCruza;
    TipoMuta m_TipoMuta;
    double m_nPc;
    double m_nProbSele;
    double m_nProbMuta;
    QVector<Individuo *> m_vIndividuo;
    std::default_random_engine generator;
};

#endif // POBLACION_H
