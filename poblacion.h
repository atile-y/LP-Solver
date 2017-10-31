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
    uint64_t getCromo(){ return m_nCromo; }
    uint getGenerations(){ return m_nGenerations; }
    TipoSeleccion getTipoSeleccion(){ return m_TipoSeleccion; }
    TipoCruza getTipoCruza(){ return m_TipoCruza; }
    TipoMuta getTipoMuta(){ return m_TipoMuta; }
    double getPC(){ return m_nPc; }
    double getProbSele(){ return m_nProbSele; }
    double getProbMuta(){ return m_nProbMuta; }
    Individuo *getIndividuo(uint);
    //
    uint getPrecision(){ return m_nPrecision; }
    bool isMaximizar(){ return m_bMaximizar; }
    QStringList getZ(){ return m_slZ; }
    QVector<QStringList> getR(){ return m_vR; }
    QString getVariables(){ return m_strVar; }
    QVector< QPair<long,long> > getLimits(){ return m_vLimits; }

    void setSize(uint s){ m_nSize = s; }
    void setCromo(uint64_t c){ m_nCromo = c; }
    void setGenerations(uint g){ m_nGenerations = g; }
    void setTipoSeleccion(TipoSeleccion t){ m_TipoSeleccion = t; }
    void setTipoCruza(TipoCruza t){ m_TipoCruza = t; }
    void setTipoMuta(TipoMuta t){ m_TipoMuta = t; }
    void setPC(double pc){ m_nPc = pc; }
    void setProbSele(double ps){ m_nProbSele = ps; }
    void setProbMuta(double pm){ m_nProbMuta = pm; }
    void setIndividuo(uint, Individuo *);
    //
    void setPrecision(uint p){ m_nPrecision = p; }
    void setMaximizar(bool m){ m_bMaximizar = m; }
    void setZ(QStringList z){ m_slZ = z; }
    void setR(QVector<QStringList> r){ m_vR = QVector<QStringList>(r); }
    void setVariables(QString v){ m_strVar = v; }
    void setLimits(QVector< QPair<long,long> > l){ m_vLimits = QVector< QPair<long,long> >(l); }

    QVector<double> evolve();

    void calcBits();
    void crearG0();
    bool valida(uint);
    double getSuma();
    double getMaximo();
    double getMinimo();

    double fitness(uint, bool = false);

    QVector<Individuo *> cruza(uint, uint);
    Individuo *muta(uint);

    void selecciona();
    void cruza();
    void muta();

private:
    QVector<uint> barajea();

    uint m_nSize;
    uint64_t m_nCromo;
    uint m_nGenerations;
    TipoSeleccion m_TipoSeleccion;
    TipoCruza m_TipoCruza;
    TipoMuta m_TipoMuta;
    double m_nPc;
    double m_nProbSele;
    double m_nProbMuta;
    QVector<Individuo *> m_vIndividuo;
    std::default_random_engine generator;
    //
    uint m_nPrecision;
    bool m_bMaximizar;
    QStringList m_slZ;
    QVector<QStringList> m_vR;
    QString m_strVar;
    QVector< QPair<long,long> > m_vLimits;
    QVector<uint> m_vBits;
};

#endif // POBLACION_H
