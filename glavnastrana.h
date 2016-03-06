#ifndef GLAVNASTRANA_H
#define GLAVNASTRANA_H

#include <iostream>
#include <fstream>
#include <stdint.h>
#include <cstdio>
#include <cstdlib>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QPushButton>
#include <QFileDialog>
#include <QDebug>
#include "dugme.h"
#include <math.h>
#include <QSlider>
#include <QLine>
#include <QGraphicsLineItem>
#include <QComboBox>
#include <QIcon>

using namespace std;

class GlavnaStrana : public QGraphicsView {
    Q_OBJECT
public:
    GlavnaStrana();
    ~GlavnaStrana();
    void obrada(int id);
    void RGB_u_HSI();
    void HSI_u_RGB();
    void ekvalizacija();
    void crtanje_histograma(int arg);
    void uprosecavanje_f();
    void medijana_f();
    void laplasijan_filter();
    void spacijalni_u_frekventni(int i, int j, int Q50[8][8], double D_Y[8][8], double D_Cb[8][8], double D_Cr[8][8],
                                 double M_Y[8][8], double M_Cb[8][8], double M_Cr[8][8], double T[8][8]);
    void kvantizacija(int i, int j, int Q50[8][8], double D_Y[8][8], double D_Cb[8][8], double D_Cr[8][8],
                      double M_Y[8][8], double M_Cb[8][8], double M_Cr[8][8], double T[8][8]);
    void frekventni_u_spacijalni(int i, int j, int Q50[8][8], double D_Y[8][8], double D_Cb[8][8], double D_Cr[8][8],
                                 double M_Y[8][8], double M_Cb[8][8], double M_Cr[8][8], double T[8][8]);
    void zatvaranje_fajlova();
    int otvaranjeFajlaZaCitanje();
    int otvaranjeFajlaZaPisanje();

    QGraphicsScene *scena;
    QGraphicsPixmapItem *slika1, *slika2;
    Dugme *ucitaj, *resetuj, *sacuvaj, *posivi, *crvena_komponenta, *zelena_komponenta, *plava_komponenta,
          *negativ, *log, *gama,*povecaj_kontrast, *binarizuj_1, *binarizuj_2, *binarizuj_3, *ekvalizuj,
          *poklopi, *nijansa, *zasicenost, *intenzitet, *uproseci, *napravi_medijanu, *izdvoj_ivice, *usm,
          *laplasijan, *sobel, *jpeg, *erodiraj, *napravi_dilataciju, *otvori, *zatvori, *s1, *s2, *s3, *s4;
    QSlider *gama_slajder, *nijansa_slajder, *zasicenost_slajder, *intenzitet_slajder, *kompresija_slajder;
    QGraphicsRectItem *pozadina_slike1, *pozadina_slike2, *pozadina_histograma1, *pozadina_histograma2;
    QLine histogram1[256], histogram2[256];
    QGraphicsLineItem *sacuvan_histogram1[256], *sacuvan_histogram2[256];
    QComboBox *izbor_histograma;
    QIcon *h1, *h2, *h3;

    struct bmpfile_magic {
      unsigned char magic[2];
    } mag;

    struct bmpfile_header {
      uint32_t filesz;
      uint16_t creator1;
      uint16_t creator2;
      uint32_t bmp_offset;
    } fh;

    struct bmpifo_header {
      uint32_t header_sz;
      int32_t width;
      int32_t height;
      uint16_t nplanes;
      uint16_t bitspp;
      uint32_t compress_type;
      uint32_t bmp_bytesz;
      int32_t hres;
      int32_t vres;
      uint32_t ncolors;
      uint32_t nimpcolors;
    } fhh;

    struct pix {
        unsigned char B, G, R, tB, tG, tR, Y, CB, CR;
        float H, S, I, tI, F_Y, F_Cb, F_Cr;
    } pic[550][550];

    bool slikaUcitana;
    char pp[3];
    char *xx, *xy, *yy;
    int pad, id_algoritma, br_strane;
    double maksimalna_gustina, gustina_raspodele[256], funkcija_raspodele[256], ciljna_g_raspodele[256],
        ciljna_f_raspodele[256], konacna_f_raspodele[256];
    QString ucitanFajl, fajlZaCuvanje;
    FILE *fp, *fp1, *fp2;

public slots:
    void inicijalizacija();
    void otvaranje_nove_slike();
    void strana1();
    void strana2();
    void strana3();
    void strana4();
    void posivljavanje();
    void pravljenje_negativa();
    void izdvajanje_crvene_komponente();
    void izdvajanje_zelene_komponente();
    void izdvajanje_plave_komponente();
    void log_transformacija();
    void gama_transformacija();
    void povecavanje_kotrasta();
    void binarizacija_1();
    void binarizacija_2();
    void binarizacija_3();
    void ekvalizacija_histograma();
    void poklapanje_histograma();
    void promena_nijanse();
    void promena_zasicenosti();
    void promena_intenziteta();
    void uprosecavanje();
    void medijana();
    void izdvajanje_ivica();
    void unsharp_masking();
    void laplasijan_izostravanje();
    void sobel_izostravanje();
    void jpeg_kompresija();
    void erozija();
    void dilatacija();
    void otvaranje();
    void zatvaranje();
    void resetovanje();
    void cuvanje();
};

#endif // GLAVNASTRANA_H
