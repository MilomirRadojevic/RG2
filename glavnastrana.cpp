#include "glavnastrana.h"

int poredjenje(const void* a, const void* b) {
    int arg1 = *reinterpret_cast<const int*>(a);
    int arg2 = *reinterpret_cast<const int*>(b);
    if (arg1 < arg2)
        return -1;
    if (arg1 > arg2)
        return 1;
    return 0;
}

GlavnaStrana::GlavnaStrana(): QGraphicsView() {
    xy = "privremeno.bmp";
    slikaUcitana = false;
    br_strane = 1;

    scena = new QGraphicsScene(this);

    scena->setSceneRect(0, 0, 1324, 640);
    setFixedSize(1324, 650);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setScene(scena);

    setStyleSheet("background-color: rgb(200, 220, 240)");

    ucitaj = new Dugme("Otvoranje nove slike");
    ucitaj->setFixedSize(258, 35);
    ucitaj->move(1055, 534);
    connect(ucitaj, SIGNAL(clicked(bool)), this, SLOT(otvaranje_nove_slike()));
    scena->addWidget(ucitaj);
}

GlavnaStrana::~GlavnaStrana() {

}

void GlavnaStrana::inicijalizacija() {
    if(slikaUcitana) {
        xx = ucitanFajl.toLatin1().data();
    }
    else {
        if(otvaranjeFajlaZaCitanje() == 0)
            return;
        else {
            slikaUcitana = true;
        }
    }

    fp = fopen(xx, "rb");

    fread(&mag, sizeof(struct bmpfile_magic), 1, fp);
    fread(&fh, sizeof(struct bmpfile_header), 1, fp);
    fread(&fhh, sizeof(struct bmpifo_header), 1, fp);

    if (fhh.bitspp != 24) {
        cout << "Nije 24bpp" << endl;
        exit(1);
    }
    pad = fhh.width % 4;
    for (int i = 0; i < fhh.height; i++) {
        for (int j = 0; j < fhh.width; j++)
            fread(&pic[i][j], 3, 1, fp);
        fread(&pp, pad, 1, fp);
    }

    pozadina_slike1 = new QGraphicsRectItem(9, 9, 513, 513);
    scena->addItem(pozadina_slike1);

    pozadina_slike2 = new QGraphicsRectItem(531, 9, 513, 513);
    scena->addItem(pozadina_slike2);

    pozadina_histograma1 = new QGraphicsRectItem(1055, 9, 256, 251);
    pozadina_histograma1->setBrush(QBrush(QColor(180, 180, 250)));
    scena->addItem(pozadina_histograma1);

    pozadina_histograma2 = new QGraphicsRectItem(1055, 271, 256, 251);
    pozadina_histograma2->setBrush(QBrush(QColor(180, 180, 250)));
    scena->addItem(pozadina_histograma2);

    slika1 = new QGraphicsPixmapItem();
    slika1->setPos(10, 10);
    slika1->setPixmap(QPixmap(ucitanFajl));
    scena->addItem(slika1);

    slika2 = new QGraphicsPixmapItem();
    slika2->setPos(532, 10);
    slika2->setPixmap(QPixmap(ucitanFajl));
    scena->addItem(slika2);

    for(int k = 0; k < 256; k++) {
        histogram1[k] = QLine();
        histogram2[k] = QLine();
        histogram1[k].setP1(QPoint(1055 + k, 260));
        histogram1[k].setP2(QPoint(1055 + k, 260));
        histogram2[k].setP1(QPoint(1055 + k, 522));
        histogram2[k].setP2(QPoint(1055 + k, 522));
        sacuvan_histogram1[k] = scena->addLine(histogram1[k], QPen(QColor(10, 30, 10)));
        sacuvan_histogram2[k] = scena->addLine(histogram2[k], QPen(QColor(10, 30, 10)));
    }
    crtanje_histograma(1);
    crtanje_histograma(2);

    s1 = new Dugme("Strana 1");
    s1->setFixedSize(80, 25);
    s1->move(10, 537);
    s1->setStyleSheet("background-color: rgb(180, 180, 240)");
    connect(s1, SIGNAL(clicked(bool)), this, SLOT(strana1()));
    scena->addWidget(s1);

    s2 = new Dugme("Strana 2");
    s2->setFixedSize(80, 25);
    s2->move(10, 562);
    s2->setStyleSheet("background-color: rgb(180, 180, 240)");
    connect(s2, SIGNAL(clicked(bool)), this, SLOT(strana2()));
    scena->addWidget(s2);

    s3 = new Dugme("Strana 3");
    s3->setFixedSize(80, 25);
    s3->move(10, 587);
    s3->setStyleSheet("background-color: rgb(180, 180, 240)");
    connect(s3, SIGNAL(clicked(bool)), this, SLOT(strana3()));
    scena->addWidget(s3);

    s4 = new Dugme("Strana 4");
    s4->setFixedSize(80, 25);
    s4->move(10, 612);
    s4->setStyleSheet("background-color: rgb(180, 180, 240)");
    connect(s4, SIGNAL(clicked(bool)), this, SLOT(strana4()));
    scena->addWidget(s4);

    posivi = new Dugme("Posivljavanje");
    posivi->setFixedSize(160, 25);
    posivi->move(210, 537);
    connect(posivi, SIGNAL(clicked(bool)), this, SLOT(posivljavanje()));
    scena->addWidget(posivi);

    negativ = new Dugme("Negativ");
    negativ->setFixedSize(160, 25);
    negativ->move(210, 597);
    connect(negativ, SIGNAL(clicked(bool)), this, SLOT(pravljenje_negativa()));
    scena->addWidget(negativ);

    crvena_komponenta = new Dugme("Crvena komponenta");
    crvena_komponenta->setFixedSize(160, 25);
    crvena_komponenta->move(410, 537);
    connect(crvena_komponenta, SIGNAL(clicked(bool)), this, SLOT(izdvajanje_crvene_komponente()));
    scena->addWidget(crvena_komponenta);

    zelena_komponenta = new Dugme("Zelena komponenta");
    zelena_komponenta->setFixedSize(160, 25);
    zelena_komponenta->move(410, 567);
    connect(zelena_komponenta, SIGNAL(clicked(bool)), this, SLOT(izdvajanje_zelene_komponente()));
    scena->addWidget(zelena_komponenta);

    plava_komponenta = new Dugme("Plava komponenta");
    plava_komponenta->setFixedSize(160, 25);
    plava_komponenta->move(410, 597);
    connect(plava_komponenta, SIGNAL(clicked(bool)), this, SLOT(izdvajanje_plave_komponente()));
    scena->addWidget(plava_komponenta);

    log = new Dugme("Log transformacija");
    log->setFixedSize(160, 25);
    log->move(610, 592);
    connect(log, SIGNAL(clicked(bool)), this, SLOT(log_transformacija()));
    scena->addWidget(log);

    gama_slajder = new QSlider(Qt::Horizontal);
    gama_slajder->setFixedSize(160, 25);
    gama_slajder->move(810, 537);
    gama_slajder->setValue(50);
    scena->addWidget(gama_slajder);
    gama = new Dugme("Gama transformacija");
    gama->setFixedSize(160, 25);
    gama->move(810, 597);
    connect(gama, SIGNAL(clicked(bool)), this, SLOT(gama_transformacija()));
    scena->addWidget(gama);

    povecaj_kontrast = new Dugme("Povecavanje kotrasta");
    povecaj_kontrast->setFixedSize(160, 25);
    povecaj_kontrast->move(610, 537);
    connect(povecaj_kontrast, SIGNAL(clicked(bool)), this, SLOT(povecavanje_kotrasta()));
    scena->addWidget(povecaj_kontrast);

    binarizuj_1 = new Dugme("Binarizacija (1 prag)");
    binarizuj_1->setFixedSize(160, 25);
    binarizuj_1->move(210, 537);
    connect(binarizuj_1, SIGNAL(clicked(bool)), this, SLOT(binarizacija_1()));
    scena->addWidget(binarizuj_1);

    binarizuj_2 = new Dugme("Binarizacija (4 praga)");
    binarizuj_2->setFixedSize(160, 25);
    binarizuj_2->move(210, 567);
    connect(binarizuj_2, SIGNAL(clicked(bool)), this, SLOT(binarizacija_2()));
    scena->addWidget(binarizuj_2);

    binarizuj_3 = new Dugme("Binarizacija (FS)");
    binarizuj_3->setFixedSize(160, 25);
    binarizuj_3->move(210, 597);
    connect(binarizuj_3, SIGNAL(clicked(bool)), this, SLOT(binarizacija_3()));
    scena->addWidget(binarizuj_3);

    ekvalizuj = new Dugme("Ekvalizacija histograma");
    ekvalizuj->setFixedSize(160, 25);
    ekvalizuj->move(810, 597);
    connect(ekvalizuj, SIGNAL(clicked(bool)), this, SLOT(ekvalizacija_histograma()));
    scena->addWidget(ekvalizuj);

    nijansa_slajder = new QSlider(Qt::Horizontal);
    nijansa_slajder->setFixedSize(160, 25);
    nijansa_slajder->move(210, 537);
    nijansa_slajder->setValue(50);
    scena->addWidget(nijansa_slajder);
    nijansa = new Dugme("Promena nijanse (HUE)");
    nijansa->setFixedSize(160, 25);
    nijansa->move(410, 537);
    connect(nijansa, SIGNAL(clicked(bool)), this, SLOT(promena_nijanse()));
    scena->addWidget(nijansa);

    zasicenost_slajder = new QSlider(Qt::Horizontal);
    zasicenost_slajder->setFixedSize(160, 25);
    zasicenost_slajder->move(210, 567);
    zasicenost_slajder->setValue(50);
    scena->addWidget(zasicenost_slajder);
    zasicenost = new Dugme("Promena zasicenosti");
    zasicenost->setFixedSize(160, 25);
    zasicenost->move(410, 567);
    connect(zasicenost, SIGNAL(clicked(bool)), this, SLOT(promena_zasicenosti()));
    scena->addWidget(zasicenost);

    intenzitet_slajder = new QSlider(Qt::Horizontal);
    intenzitet_slajder->setFixedSize(160, 25);
    intenzitet_slajder->move(210, 597);
    intenzitet_slajder->setValue(50);
    scena->addWidget(intenzitet_slajder);
    intenzitet = new Dugme("Promena intenziteta");
    intenzitet->setFixedSize(160, 25);
    intenzitet->move(410, 597);
    connect(intenzitet, SIGNAL(clicked(bool)), this, SLOT(promena_intenziteta()));
    scena->addWidget(intenzitet);

    h1 = new QIcon(QString(":/1.png"));
    h2 = new QIcon(QString(":/2.png"));
    h3 = new QIcon(QString(":/3.png"));
    izbor_histograma = new QComboBox();
    izbor_histograma->addItem(*h1, "");
    izbor_histograma->addItem(*h2, "");
    izbor_histograma->addItem(*h3, "");
    izbor_histograma->move(610, 537);
    izbor_histograma->setFixedSize(80, 30);
    izbor_histograma->setIconSize(QSize(30, 20));
    scena->addWidget(izbor_histograma);

    poklopi = new Dugme("Histogram matching");
    poklopi->setFixedSize(160, 25);
    poklopi->move(810, 537);
    connect(poklopi, SIGNAL(clicked(bool)), this, SLOT(poklapanje_histograma()));
    scena->addWidget(poklopi);

    uproseci = new Dugme("Uprosecavanje");
    uproseci->setFixedSize(160, 25);
    uproseci->move(210, 537);
    connect(uproseci, SIGNAL(clicked(bool)), this, SLOT(uprosecavanje()));
    scena->addWidget(uproseci);

    napravi_medijanu = new Dugme("Medijana");
    napravi_medijanu->setFixedSize(160, 25);
    napravi_medijanu->move(210, 597);
    connect(napravi_medijanu, SIGNAL(clicked(bool)), this, SLOT(medijana()));
    scena->addWidget(napravi_medijanu);

    izdvoj_ivice = new Dugme("Izdvajanje ivica");
    izdvoj_ivice->setFixedSize(160, 25);
    izdvoj_ivice->move(410, 537);
    connect(izdvoj_ivice, SIGNAL(clicked(bool)), this, SLOT(izdvajanje_ivica()));
    scena->addWidget(izdvoj_ivice);

    usm = new Dugme("Unsharp masking");
    usm->setFixedSize(160, 25);
    usm->move(410, 597);
    connect(usm, SIGNAL(clicked(bool)), this, SLOT(unsharp_masking()));
    scena->addWidget(usm);

    laplasijan = new Dugme("Laplasijan");
    laplasijan->setFixedSize(160, 25);
    laplasijan->move(610, 537);
    connect(laplasijan, SIGNAL(clicked(bool)), this, SLOT(laplasijan_izostravanje()));
    scena->addWidget(laplasijan);

    sobel = new Dugme("Sobelov operator");
    sobel->setFixedSize(160, 25);
    sobel->move(610, 597);
    connect(sobel, SIGNAL(clicked(bool)), this, SLOT(sobel_izostravanje()));
    scena->addWidget(sobel);

    kompresija_slajder = new QSlider(Qt::Horizontal);
    kompresija_slajder->setFixedSize(160, 25);
    kompresija_slajder->move(810, 537);
    kompresija_slajder->setValue(50);
    scena->addWidget(kompresija_slajder);
    jpeg = new Dugme("JPEG kompresija");
    jpeg->setFixedSize(160, 25);
    jpeg->move(810, 597);
    connect(jpeg, SIGNAL(clicked(bool)), this, SLOT(jpeg_kompresija()));
    scena->addWidget(jpeg);

    erodiraj = new Dugme("Erozija");
    erodiraj->setFixedSize(160, 25);
    erodiraj->move(510, 537);
    connect(erodiraj, SIGNAL(clicked(bool)), this, SLOT(erozija()));
    scena->addWidget(erodiraj);

    napravi_dilataciju = new Dugme("Dilatacija");
    napravi_dilataciju->setFixedSize(160, 25);
    napravi_dilataciju->move(510, 597);
    connect(napravi_dilataciju, SIGNAL(clicked(bool)), this, SLOT(dilatacija()));
    scena->addWidget(napravi_dilataciju);

    otvori = new Dugme("Otvaranje");
    otvori->setFixedSize(160, 25);
    otvori->move(810, 537);
    connect(otvori, SIGNAL(clicked(bool)), this, SLOT(otvaranje()));
    scena->addWidget(otvori);

    zatvori = new Dugme("Zatvaranje");
    zatvori->setFixedSize(160, 25);
    zatvori->move(810, 597);
    connect(zatvori, SIGNAL(clicked(bool)), this, SLOT(zatvaranje()));
    scena->addWidget(zatvori);

    resetuj = new Dugme("Resetovanje");
    resetuj->setFixedSize(258, 35);
    resetuj->move(1055, 606);
    connect(resetuj, SIGNAL(clicked(bool)), this, SLOT(resetovanje()));
    scena->addWidget(resetuj);

    sacuvaj = new Dugme("Cuvanje");
    sacuvaj->setFixedSize(258, 35);
    sacuvaj->move(1055, 570);
    connect(sacuvaj, SIGNAL(clicked(bool)), this, SLOT(cuvanje()));
    scena->addWidget(sacuvaj);

    switch (br_strane) {
        case 1:
            strana1();
            break;
        case 2:
            strana2();
            break;
        case 3:
            strana3();
            break;
        case 4:
            strana4();
            break;
        default:
            break;
    }
}

void GlavnaStrana::otvaranje_nove_slike() {
    if(slikaUcitana) {
        posivi->setVisible(false);
        crvena_komponenta->setVisible(false);
        zelena_komponenta->setVisible(false);
        plava_komponenta->setVisible(false);
        negativ->setVisible(false);
        log->setVisible(false);
        gama->setVisible(false);
        povecaj_kontrast->setVisible(false);
        binarizuj_1->setVisible(false);
        binarizuj_2->setVisible(false);
        binarizuj_3->setVisible(false);
        ekvalizuj->setVisible(false);
        poklopi->setVisible(false);
        nijansa->setVisible(false);
        zasicenost->setVisible(false);
        intenzitet->setVisible(false);
        uproseci->setVisible(false);
        napravi_medijanu->setVisible(false);
        izdvoj_ivice->setVisible(false);
        usm->setVisible(false);
        laplasijan->setVisible(false);
        sobel->setVisible(false);
        jpeg->setVisible(false);
        erodiraj->setVisible(false);
        napravi_dilataciju->setVisible(false);
        otvori->setVisible(false);
        zatvori->setVisible(false);
        gama_slajder->setVisible(false);
        nijansa_slajder->setVisible(false);
        zasicenost_slajder->setVisible(false);
        intenzitet_slajder->setVisible(false);
        kompresija_slajder->setVisible(false);
        izbor_histograma->setVisible(false);
    }
    slikaUcitana = false;
    inicijalizacija();
}

void GlavnaStrana::strana1() {
    posivi->setVisible(true);
    crvena_komponenta->setVisible(true);
    zelena_komponenta->setVisible(true);
    plava_komponenta->setVisible(true);
    negativ->setVisible(true);
    log->setVisible(true);
    gama->setVisible(true);
    povecaj_kontrast->setVisible(true);
    binarizuj_1->setVisible(false);
    binarizuj_2->setVisible(false);
    binarizuj_3->setVisible(false);
    ekvalizuj->setVisible(false);
    poklopi->setVisible(false);
    nijansa->setVisible(false);
    zasicenost->setVisible(false);
    intenzitet->setVisible(false);
    uproseci->setVisible(false);
    napravi_medijanu->setVisible(false);
    izdvoj_ivice->setVisible(false);
    usm->setVisible(false);
    laplasijan->setVisible(false);
    sobel->setVisible(false);
    jpeg->setVisible(false);
    erodiraj->setVisible(false);
    napravi_dilataciju->setVisible(false);
    otvori->setVisible(false);
    zatvori->setVisible(false);
    gama_slajder->setVisible(true);
    nijansa_slajder->setVisible(false);
    zasicenost_slajder->setVisible(false);
    intenzitet_slajder->setVisible(false);
    kompresija_slajder->setVisible(false);
    izbor_histograma->setVisible(false);

    br_strane = 1;
}

void GlavnaStrana::strana2() {
    posivi->setVisible(false);
    crvena_komponenta->setVisible(false);
    zelena_komponenta->setVisible(false);
    plava_komponenta->setVisible(false);
    negativ->setVisible(false);
    log->setVisible(false);
    gama->setVisible(false);
    povecaj_kontrast->setVisible(false);
    binarizuj_1->setVisible(false);
    binarizuj_2->setVisible(false);
    binarizuj_3->setVisible(false);
    ekvalizuj->setVisible(true);
    poklopi->setVisible(true);
    nijansa->setVisible(true);
    zasicenost->setVisible(true);
    intenzitet->setVisible(true);
    uproseci->setVisible(false);
    napravi_medijanu->setVisible(false);
    izdvoj_ivice->setVisible(false);
    usm->setVisible(false);
    laplasijan->setVisible(false);
    sobel->setVisible(false);
    jpeg->setVisible(false);
    erodiraj->setVisible(false);
    napravi_dilataciju->setVisible(false);
    otvori->setVisible(false);
    zatvori->setVisible(false);
    gama_slajder->setVisible(false);
    nijansa_slajder->setVisible(true);
    zasicenost_slajder->setVisible(true);
    intenzitet_slajder->setVisible(true);
    kompresija_slajder->setVisible(false);
    izbor_histograma->setVisible(true);

    br_strane = 2;
}

void GlavnaStrana::strana3() {
    posivi->setVisible(false);
    crvena_komponenta->setVisible(false);
    zelena_komponenta->setVisible(false);
    plava_komponenta->setVisible(false);
    negativ->setVisible(false);
    log->setVisible(false);
    gama->setVisible(false);
    povecaj_kontrast->setVisible(false);
    binarizuj_1->setVisible(false);
    binarizuj_2->setVisible(false);
    binarizuj_3->setVisible(false);
    ekvalizuj->setVisible(false);
    poklopi->setVisible(false);
    nijansa->setVisible(false);
    zasicenost->setVisible(false);
    intenzitet->setVisible(false);
    uproseci->setVisible(true);
    napravi_medijanu->setVisible(true);
    izdvoj_ivice->setVisible(true);
    usm->setVisible(true);
    laplasijan->setVisible(true);
    sobel->setVisible(true);
    jpeg->setVisible(true);
    erodiraj->setVisible(false);
    napravi_dilataciju->setVisible(false);
    otvori->setVisible(false);
    zatvori->setVisible(false);
    gama_slajder->setVisible(false);
    nijansa_slajder->setVisible(false);
    zasicenost_slajder->setVisible(false);
    intenzitet_slajder->setVisible(false);
    kompresija_slajder->setVisible(true);
    izbor_histograma->setVisible(false);

    br_strane = 3;
}

void GlavnaStrana::strana4() {
    posivi->setVisible(false);
    crvena_komponenta->setVisible(false);
    zelena_komponenta->setVisible(false);
    plava_komponenta->setVisible(false);
    negativ->setVisible(false);
    log->setVisible(false);
    gama->setVisible(false);
    povecaj_kontrast->setVisible(false);
    binarizuj_1->setVisible(true);
    binarizuj_2->setVisible(true);
    binarizuj_3->setVisible(true);
    ekvalizuj->setVisible(false);
    poklopi->setVisible(false);
    nijansa->setVisible(false);
    zasicenost->setVisible(false);
    intenzitet->setVisible(false);
    uproseci->setVisible(false);
    napravi_medijanu->setVisible(false);
    izdvoj_ivice->setVisible(false);
    usm->setVisible(false);
    laplasijan->setVisible(false);
    sobel->setVisible(false);
    jpeg->setVisible(false);
    erodiraj->setVisible(true);
    napravi_dilataciju->setVisible(true);
    otvori->setVisible(true);
    zatvori->setVisible(true);
    gama_slajder->setVisible(false);
    nijansa_slajder->setVisible(false);
    zasicenost_slajder->setVisible(false);
    intenzitet_slajder->setVisible(false);
    kompresija_slajder->setVisible(false);
    izbor_histograma->setVisible(false);

    br_strane = 4;
}

void GlavnaStrana::posivljavanje() {
    obrada(1);
}

void GlavnaStrana::pravljenje_negativa() {
    obrada(2);
}

void GlavnaStrana::izdvajanje_crvene_komponente() {
    obrada(3);
}

void GlavnaStrana::izdvajanje_plave_komponente() {
    obrada(4);
}

void GlavnaStrana::izdvajanje_zelene_komponente() {
    obrada(5);
}

void GlavnaStrana::log_transformacija() {
    obrada(6);
}

void GlavnaStrana::gama_transformacija() {
    obrada(7);
}

void GlavnaStrana::povecavanje_kotrasta() {
    obrada(8);
}

void GlavnaStrana::binarizacija_1() {
    obrada(9);
}

void GlavnaStrana::binarizacija_2() {
    obrada(10);
}

void GlavnaStrana::binarizacija_3() {
    obrada(11);
}

void GlavnaStrana::ekvalizacija_histograma() {
    obrada(12);
}

void GlavnaStrana::promena_nijanse() {
    obrada(13);
}

void GlavnaStrana::promena_zasicenosti() {
    obrada(14);
}

void GlavnaStrana::promena_intenziteta() {
    obrada(15);
}

void GlavnaStrana::poklapanje_histograma() {
    obrada(16);
}

void GlavnaStrana::uprosecavanje() {
    obrada(17);
}

void GlavnaStrana::medijana() {
    obrada(18);
}

void GlavnaStrana::izdvajanje_ivica() {
    obrada(19);
}

void GlavnaStrana::unsharp_masking() {
    obrada(20);
}

void GlavnaStrana::laplasijan_izostravanje() {
    obrada(21);
}

void GlavnaStrana::sobel_izostravanje() {
    obrada(22);
}

void GlavnaStrana::jpeg_kompresija() {
    obrada(23);
}

void GlavnaStrana::erozija() {
    obrada(24);
}

void GlavnaStrana::dilatacija() {
    obrada(25);
}

void GlavnaStrana::otvaranje() {
    obrada(26);
}

void GlavnaStrana::zatvaranje() {
    obrada(27);
}

void GlavnaStrana::resetovanje() {
    posivi->setVisible(false);
    crvena_komponenta->setVisible(false);
    zelena_komponenta->setVisible(false);
    plava_komponenta->setVisible(false);
    negativ->setVisible(false);
    log->setVisible(false);
    gama->setVisible(false);
    povecaj_kontrast->setVisible(false);
    binarizuj_1->setVisible(false);
    binarizuj_2->setVisible(false);
    binarizuj_3->setVisible(false);
    ekvalizuj->setVisible(false);
    poklopi->setVisible(false);
    nijansa->setVisible(false);
    zasicenost->setVisible(false);
    intenzitet->setVisible(false);
    uproseci->setVisible(false);
    napravi_medijanu->setVisible(false);
    izdvoj_ivice->setVisible(false);
    usm->setVisible(false);
    laplasijan->setVisible(false);
    sobel->setVisible(false);
    jpeg->setVisible(false);
    erodiraj->setVisible(false);
    napravi_dilataciju->setVisible(false);
    otvori->setVisible(false);
    zatvori->setVisible(false);
    gama_slajder->setVisible(false);
    nijansa_slajder->setVisible(false);
    zasicenost_slajder->setVisible(false);
    intenzitet_slajder->setVisible(false);
    kompresija_slajder->setVisible(false);
    izbor_histograma->setVisible(false);
    zatvaranje_fajlova();
    slikaUcitana = true;
    inicijalizacija();
}

void GlavnaStrana::obrada(int id) {
    int v, u = 0;
    double ukupna_suma = 0, T[8][8] = {{0.3536, 0.3536, 0.3536, 0.3536, 0.3536, 0.3536, 0.3536, 0.3536},
                                       {0.4904, 0.4157, 0.2778, 0.0975, -0.0975, -0.2778, -0.4157, -0.4904},
                                       {0.4619, 0.1913, -0.1913, -0.4619, -0.4619, -0.1913, 0.1913, 0.4619},
                                       {0.4157, -0.0975, -0.4904, -0.2778, 0.2778, 0.4904, 0.0975, -0.4157},
                                       {0.3536, -0.3536, -0.3536, 0.3536, 0.3536, -0.3536, -0.3536, 0.3536},
                                       {0.2728, -0.4904, 0.0975, 0.4157, -0.4157, -0.0975, 0.4904, -0.2778},
                                       {0.1913, -0.4619, 0.4619, -0.1913, -0.1913, 0.4619, -0.4619, 0.1913},
                                       {0.0975, -0.2778, 0.4157, -0.4904, 0.4904, -0.4157, 0.2778, -0.0975}},
            M_Y[8][8], M_Cb[8][8],M_Cr[8][8],D_Y[8][8],D_Cb[8][8],D_Cr[8][8];
    int Q50[8][8] = {{16, 11, 10, 16, 24, 40, 51, 61},
                     {12, 12, 14, 19, 26, 58, 60, 55},
                     {14, 13, 16, 24, 40, 57, 69, 56},
                     {14, 17, 22, 29, 51, 87, 80, 62},
                     {18, 22, 37, 56, 68, 109, 103, 77},
                     {24, 35, 55, 64, 81, 104, 113, 92},
                     {49, 64, 78, 87, 103, 121, 120, 101},
                     {72, 92, 95, 98, 112, 100, 103, 99}};
    switch(id) {
        case 1:
            for (int i = 0; i < fhh.height; i++) {
                for (int j = 0; j < fhh.width; j++) {
                    v = (pic[i][j].R + pic[i][j].G + pic[i][j].B + 1) / 3;
                    pic[i][j].R = v;
                    pic[i][j].G = v;
                    pic[i][j].B = v;

                }
            }
            break;
        case 2:
            for (int i = 0; i < fhh.height; i++) {
                for (int j = 0; j < fhh.width; j++) {
                    pic[i][j].R = 255 - pic[i][j].R;
                    pic[i][j].G = 255 - pic[i][j].G;
                    pic[i][j].B = 255 - pic[i][j].B;
                }
            }
            break;
        case 3:
            for (int i = 0; i < fhh.height; i++) {
                for (int j = 0; j < fhh.width; j++) {
                    pic[i][j].G = 0;
                    pic[i][j].B = 0;
                }
            }
            break;
        case 4:
            for (int i = 0; i < fhh.height; i++) {
                for (int j = 0; j < fhh.width; j++) {
                    pic[i][j].R = 0;
                    pic[i][j].G = 0;
                }
            }
            break;
        case 5:
            for (int i = 0; i < fhh.height; i++) {
                for (int j = 0; j < fhh.width; j++) {
                    pic[i][j].R = 0;
                    pic[i][j].B = 0;
                }
            }
            break;
        case 6:
            for (int i = 0; i < fhh.height; i++) {
                for (int j = 0; j < fhh.width; j++) {
                    pic[i][j].R = 32 * log2(pic[i][j].R + 0.0);
                    pic[i][j].G = 32 * log2(pic[i][j].G + 0.0);
                    pic[i][j].B = 32 * log2(pic[i][j].B + 0.0);
                }
            }
            break;
        case 7:
            v = gama_slajder->value();
            double ga;
            if(v >= 50) {
                ga = 1.0 + (v - 50.0) * 0.2;
            }
            else {
                ga = 1.0 / (1.0 + (49.0 - v) * 0.2);
            }
            for (int i = 0; i < fhh.height; i++) {
                for (int j = 0; j < fhh.width; j++) {
                    pic[i][j].R = 255 * pow((pic[i][j].R / 255.0), ga);
                    pic[i][j].G = 255 * pow((pic[i][j].G / 255.0), ga);
                    pic[i][j].B = 255 * pow((pic[i][j].B / 255.0), ga);
                }
            }
            break;
        case 8:
            for (int i = 0; i < fhh.height; i++) {
                for (int j = 0; j < fhh.width; j++) {
                    if (pic[i][j].R < 96)
                        pic[i][j].R /= 3;
                    else if (pic[i][j].R < 160)
                        pic[i][j].R = 32 + (pic[i][j].R - 96) * 3;
                    else
                        pic[i][j].R = 224 + (pic[i][j].R - 160) / 3;
                    if (pic[i][j].G < 96)
                        pic[i][j].G /= 3;
                    else if (pic[i][j].G < 160)
                        pic[i][j].G = 32 + (pic[i][j].G - 96) * 3;
                    else
                        pic[i][j].G = 224 + (pic[i][j].G - 160) / 3;
                    if (pic[i][j].B < 96)
                        pic[i][j].B /= 3;
                    else if (pic[i][j].B < 160)
                        pic[i][j].B = 32 + (pic[i][j].B - 96) * 3;
                    else
                        pic[i][j].B = 224 + (pic[i][j].B - 160) / 3;
                }
            }
            break;
        case 9:
            for (int i = 0; i < fhh.height; i++)
                for (int j = 0; j < fhh.width; j++)
                    u += (pic[i][j].R + pic[i][j].G + pic[i][j].B + 1) / 3;
            u /= fhh.height * fhh.width;
            for (int i = 0; i < fhh.height; i++) {
                for (int j = 0; j < fhh.width; j++) {
                    v = (pic[i][j].R + pic[i][j].G + pic[i][j].B + 1) / 3;
                    pic[i][j].R = v > u ? 255 : 0;
                    pic[i][j].G = v > u ? 255 : 0;
                    pic[i][j].B = v > u ? 255 : 0;
                }
            }
            break;
        case 10:
            for (int i = 0; i < fhh.height; i += 2) {
                for (int j = 0; j < fhh.width; j += 2) {
                    v = (pic[i][j].R + pic[i][j].G + pic[i][j].B +
                         pic[i + 1][j].R + pic[i + 1][j].G + pic[i + 1][j].B +
                         pic[i][j + 1].R + pic[i][j + 1].G + pic[i][j + 1].B +
                         pic[i + 1][j + 1].R + pic[i + 1][j + 1].G + pic[i + 1][j + 1].B + 4) / 12;;
                    if (v < 52) {
                        pic[i][j].R = pic[i][j].G = pic[i][j].B = 0;
                        pic[i + 1][j].R = pic[i + 1][j].G = pic[i + 1][j].B = 0;
                        pic[i][j + 1].R = pic[i][j + 1].G = pic[i][j + 1].B = 0;
                        pic[i + 1][j + 1].R = pic[i + 1][j + 1].G = pic[i + 1][j + 1].B = 0;
                    }
                    else if (v < 103) {
                        pic[i][j].R = pic[i][j].G = pic[i][j].B = 255;
                        pic[i + 1][j].R = pic[i + 1][j].G = pic[i + 1][j].B = 0;
                        pic[i][j + 1].R = pic[i][j + 1].G = pic[i][j + 1].B = 0;
                        pic[i + 1][j + 1].R = pic[i + 1][j + 1].G = pic[i + 1][j + 1].B = 0;
                    }
                    else if (v < 154) {
                        pic[i][j].R = pic[i][j].G = pic[i][j].B = 255;
                        pic[i + 1][j].R = pic[i + 1][j].G = pic[i + 1][j].B = 0;
                        pic[i][j + 1].R = pic[i][j + 1].G = pic[i][j + 1].B = 0;
                        pic[i + 1][j + 1].R = pic[i + 1][j + 1].G = pic[i + 1][j + 1].B = 255;
                    }
                    else if (v < 205) {
                        pic[i][j].R = pic[i][j].G = pic[i][j].B = 255;
                        pic[i + 1][j].R = pic[i + 1][j].G = pic[i + 1][j].B = 0;
                        pic[i][j + 1].R = pic[i][j + 1].G = pic[i][j + 1].B = 255;
                        pic[i + 1][j + 1].R = pic[i + 1][j + 1].G = pic[i + 1][j + 1].B = 255;
                    }
                    else {
                        pic[i][j].R = pic[i][j].G = pic[i][j].B = 255;
                        pic[i + 1][j].R = pic[i + 1][j].G = pic[i + 1][j].B = 255;
                        pic[i][j + 1].R = pic[i][j + 1].G = pic[i][j + 1].B = 255;
                        pic[i + 1][j + 1].R = pic[i + 1][j + 1].G = pic[i + 1][j + 1].B = 255;
                    }
                }
            }
            break;
        case 11:
            for (int i = 0; i < fhh.height; i++)
                for (int j = 0; j < fhh.width; j++)
                    u += (pic[i][j].R + pic[i][j].G + pic[i][j].B + 1) / 3;
            u /= fhh.height * fhh.width;
            for (int i = 0; i < fhh.height; i++) {
                for (int j = 0; j < fhh.width; j++) {
                    v = (pic[i][j].R + pic[i][j].G + pic[i][j].B + 1) / 3;
                    if( v > u) {
                        int x = 255 - v;
                        pic[i + 1][j].R = (pic[i + 1][j].R - 5.0 / 16.0 * x) < 0 ? 0 : (pic[i + 1][j].R - 5.0 / 16.0 * x);
                        pic[i][j + 1].R = (pic[i][j + 1].R - 7.0 / 16.0 * x) < 0 ? 0 : (pic[i][j + 1].R - 7.0 / 16.0 * x);
                        pic[i + 1][j + 1].R = (pic[i + 1][j + 1].R - 1.0 / 16.0 * x) < 0 ? 0 : (pic[i + 1][j + 1].R - 1.0 / 16.0 * x);
                        if(j > 0)
                            pic[i + 1][j - 1].R = (pic[i + 1][j - 1].R - 3.0 / 16.0 * x) < 0 ? 0 : (pic[i + 1][j - 1].R - 3.0 / 16.0 * x);
                        pic[i][j].R = 255;
                        pic[i + 1][j].G = (pic[i + 1][j].G - 5.0 / 16.0 * x) < 0 ? 0 : (pic[i + 1][j].G - 5.0 / 16.0 * x);
                        pic[i][j + 1].G = (pic[i][j + 1].G - 7.0 / 16.0 * x) < 0 ? 0 : (pic[i][j + 1].G - 7.0 / 16.0 * x);
                        pic[i + 1][j + 1].G = (pic[i + 1][j + 1].G - 1.0 / 16.0 * x) < 0 ? 0 : (pic[i + 1][j + 1].G - 1.0 / 16.0 * x);
                        if(j > 0)
                            pic[i + 1][j - 1].G = (pic[i + 1][j - 1].G - 3.0 / 16.0 * x) < 0 ? 0 : (pic[i + 1][j - 1].G - 3.0 / 16.0 * x);
                        pic[i][j].G = 255;
                        pic[i + 1][j].B = (pic[i + 1][j].B - 5.0 / 16.0 * x) < 0 ? 0 : (pic[i + 1][j].B - 5.0 / 16.0 * x);
                        pic[i][j + 1].B = (pic[i][j + 1].B - 7.0 / 16.0 * x) < 0 ? 0 : (pic[i][j + 1].B - 7.0 / 16.0 * x);
                        pic[i + 1][j + 1].B = (pic[i + 1][j + 1].B - 1.0 / 16.0 * x) < 0 ? 0 : (pic[i + 1][j + 1].B - 1.0 / 16.0 * x);
                        if(j > 0)
                            pic[i + 1][j - 1].B = (pic[i + 1][j - 1].B - 3.0 / 16.0 * x) < 0 ? 0 : (pic[i + 1][j - 1].B - 3.0 / 16.0 * x);
                        pic[i][j].B = 255;
                    }
                    else {
                        int x = v;
                        pic[i + 1][j].R = (int)(pic[i + 1][j].R + 5.0 / 16.0 * x) % 256;
                        pic[i][j + 1].R = (int)(pic[i][j + 1].R + 7.0 / 16.0 * x) % 256;
                        pic[i + 1][j + 1].R = (int)(pic[i + 1][j + 1].R + 1.0 / 16.0 * x) % 256;
                        if(j > 0)
                            pic[i + 1][j - 1].R = (int)(pic[i + 1][j - 1].R + 3.0 / 16.0 * x) % 256;
                        pic[i][j].R = 0;
                        pic[i + 1][j].G = (int)(pic[i + 1][j].G + 5.0 / 16.0 * x) % 256;
                        pic[i][j + 1].G = (int)(pic[i][j + 1].G + 7.0 / 16.0 * x) % 256;
                        pic[i + 1][j + 1].G = (int)(pic[i + 1][j + 1].G + 1.0 / 16.0 * x) % 256;
                        if(j > 0)
                            pic[i + 1][j - 1].G = (int)(pic[i + 1][j - 1].G + 3.0 / 16.0 * x) % 256;
                        pic[i][j].G = 0;
                        pic[i + 1][j].B = (int)(pic[i + 1][j].B + 5.0 / 16.0 * x) % 256;
                        pic[i][j + 1].B = (int)(pic[i][j + 1].B + 7.0 / 16.0 * x) % 256;
                        pic[i + 1][j + 1].B = (int)(pic[i + 1][j + 1].B + 1.0 / 16.0 * x) % 256;
                        if(j > 0)
                            pic[i + 1][j - 1].B = (int)(pic[i + 1][j - 1].B + 3.0 / 16.0 * x) % 256;
                        pic[i][j].B = 0;
                    }
                }
            }
            break;
        case 12:
            RGB_u_HSI();
            ekvalizacija();
            HSI_u_RGB();
            break;
        case 13:
            RGB_u_HSI();
            for (int i = 0; i < fhh.height; i++)
                for (int j = 0; j < fhh.width; j++)
                    pic[i][j].H = nijansa_slajder->value() / 49.5 * M_PI;
            HSI_u_RGB();
            break;
        case 14:
            RGB_u_HSI();
            for (int i = 0; i < fhh.height; i++)
                for (int j = 0; j < fhh.width; j++)
                    pic[i][j].S = zasicenost_slajder->value() / 99.0;
            HSI_u_RGB();
            break;
        case 15:
            RGB_u_HSI();
            for (int i = 0; i < fhh.height; i++)
                for (int j = 0; j < fhh.width; j++)
                    pic[i][j].I = intenzitet_slajder->value() / 99.0;
            HSI_u_RGB();
            break;
        case 16:
            RGB_u_HSI();
            for (int k = 0; k < 256; k++) {
                funkcija_raspodele[k] = 0;
            }
            for (int k = 0; k < 256; k++) {
                for(int l = 0; l < k; l++) {
                    funkcija_raspodele[k] += gustina_raspodele[l];
                }
            }
            int k;
            switch(izbor_histograma->currentIndex()) {
                case 0:
                    for (k = 0; k < 256; k++) {
                        ciljna_g_raspodele[k] = k / 255.0;
                        ukupna_suma += ciljna_g_raspodele[k];
                    }
                    break;
                case 1:
                    for (k = 0; k < 256; k++) {
                        ciljna_g_raspodele[k] = 1 - k / 255.0;
                        ukupna_suma += ciljna_g_raspodele[k];
                    }
                    break;
                case 2:
                    for (k = 0; k < 39; k++) {
                        ciljna_g_raspodele[k] = 0;
                    }
                    for (k = 39; k < 218; k++) {
                        ciljna_g_raspodele[k] = (sin(((k / 255.0) - 0.33) * 9) + 1) / 2;
                        ukupna_suma += ciljna_g_raspodele[k];
                    }
                    for(k = 218; k < 256; k++) {
                        ciljna_g_raspodele[k] = 0;
                    }
                    break;
            }
            for (k = 0; k < 256; k++) {
                ciljna_g_raspodele[k] /= ukupna_suma;
            }

            for (int k = 0; k < 256; k++) {
                ciljna_f_raspodele[k] = 0;
            }
            for (int k = 0; k < 256; k++) {
                for(int l = 0; l < k; l++) {
                    ciljna_f_raspodele[k] += ciljna_g_raspodele[l];
                }
            }
            for (int k = 0; k < 256; k++) {
                double min = 1000;
                for (int l = 0; l < 256; l++) {
                    if(abs((int)round(funkcija_raspodele[k] * 255) - (int)round(ciljna_f_raspodele[l] * 255)) < min)
                        min = abs((int)round(funkcija_raspodele[k] * 255) - (int)round(ciljna_f_raspodele[l] * 255));
                }
                for (int l = 0; l < 256; l++) {
                    if(abs((int)round(funkcija_raspodele[k] * 255) - (int)round(ciljna_f_raspodele[l] * 255)) == min) {
                        konacna_f_raspodele[k] = ciljna_f_raspodele[l];
                        break;
                    }
                }
            }
            for (int i = 0; i < fhh.height; i++) {
                for (int j = 0; j < fhh.width; j++) {
                    pic[i][j].I = konacna_f_raspodele[(int)round(pic[i][j].I * 255)];
                }
            }
            HSI_u_RGB();
            break;
        case 17:
            uprosecavanje_f();
            RGB_u_HSI();
            for (int i = 1; i < fhh.height - 1; i++) {
                for (int j = 1; j < fhh.width - 1; j++) {
                    pic[i][j].I = pic[i][j].tI;
                }
            }
            HSI_u_RGB();
            break;
        case 18:
            RGB_u_HSI();
            medijana_f();
            for (int i = 1; i < fhh.height - 1; i++) {
                for (int j = 1; j < fhh.width - 1; j++) {
                    pic[i][j].I = pic[i][j].tI;
                }
            }
            HSI_u_RGB();
            break;
        case 19:
            uprosecavanje_f();
            RGB_u_HSI();
            for (int i = 1; i < fhh.height - 1; i++) {
                for (int j = 1; j < fhh.width - 1; j++) {
                    pic[i][j].I = fabs(pic[i][j].I - pic[i][j].tI);
                }
            }
            HSI_u_RGB();
            break;
        case 20:
            uprosecavanje_f();
            RGB_u_HSI();
            for (int i = 1; i < fhh.height - 1; i++) {
                for (int j = 1; j < fhh.width - 1; j++) {
                    pic[i][j].I += fabs(pic[i][j].I - pic[i][j].tI);
                }
            }
            HSI_u_RGB();
            break;
        case 21:
            laplasijan_filter();
            RGB_u_HSI();
            for (int i = 1; i < fhh.height - 1; i++) {
                for (int j = 1; j < fhh.width - 1; j++) {
                    pic[i][j].I += fabs(pic[i][j].I - pic[i][j].tI) / 2.0/*!*/;
                }
            }
            HSI_u_RGB();
            break;
        case 22:
            RGB_u_HSI();
            double gx, gy, suma;
            for (int i = 1; i < fhh.height - 1; i++) {
                for (int j = 1; j < fhh.width - 1; j++) {
                    gx = pic[i - 1][j - 1].I + 2 * pic[i][j - 1].I + pic[i + 1][j - 1].I -
                         pic[i - 1][j + 1].I - 2 * pic[i][j + 1].I - pic[i + 1][j + 1].I;
                    gy = pic[i - 1][j - 1].I + 2 * pic[i - 1][j].I + pic[i - 1][j + 1].I -
                         pic[i + 1][j - 1].I - 2 * pic[i + 1][j].I - pic[i + 1][j + 1].I;
                    suma = fabs(gx) + fabs(gy);
                    suma = suma > 255 ? 255 : suma;
                    suma = suma < 0 ? 0 : suma;
                    pic[i][j].tI = suma;
                }
            }
            for (int i = 1; i < fhh.height - 1; i++) {
                for (int j = 1; j < fhh.width - 1; j++) {
                    pic[i][j].I += pic[i][j].tI / 5.0/*!*/;
                }
            }
            HSI_u_RGB();
            break;
        case 23:
            for (int i = 0; i < fhh.height; i++) {
                for (int j = 0; j < fhh.width; j++) {
                    pic[i][j].Y  = (int)( 0.299 * pic[i][j].R + 0.587 * pic[i][j].G + 0.114 * pic[i][j].B);
                    pic[i][j].CB = (int)(-0.169 * pic[i][j].R - 0.331 * pic[i][j].G + 0.500 * pic[i][j].B) + 128;
                    pic[i][j].CR = (int)( 0.500 * pic[i][j].R - 0.419 * pic[i][j].G - 0.081 * pic[i][j].B) + 128;
                }
            }
            for (int i = 0; i < fhh.height; i += 8) {
                for (int j = 0; j < fhh.width; j += 8) {
                    spacijalni_u_frekventni(i, j, Q50, D_Y, D_Cb, D_Cr, M_Y, M_Cb, M_Cr, T);
                    kvantizacija(i, j, Q50, D_Y, D_Cb, D_Cr, M_Y, M_Cb, M_Cr, T);
                    frekventni_u_spacijalni(i, j, Q50, D_Y, D_Cb, D_Cr, M_Y, M_Cb, M_Cr, T);
                }
            }
            for (int i = 0; i < fhh.height; i++) {
                for (int j = 0; j < fhh.width; j++) {
                    pic[i][j].R = pic[i][j].Y + 1.4 * (pic[i][j].CR - 128);
                    pic[i][j].G = pic[i][j].Y - 0.343 * (pic[i][j].CB - 128) - 0.711 * (pic[i][j].CR - 128);
                    pic[i][j].B = pic[i][j].Y + 1.765 * (pic[i][j].CB - 128);
                }
            }
            break;
        case 24:
            RGB_u_HSI();
            for (int i = 1; i < fhh.height - 1; i++) {
                for (int j = 1; j < fhh.width - 1; j++) {
                    if (pic[i - 1][j].I == 0 || pic[i][j - 1].I == 0 || pic[i][j].I == 0 || pic[i][j + 1].I == 0 || pic[i + 1][j].I == 0)
                        pic[i][j].tI = 0;
                    else
                        pic[i][j].tI = 1;
                }
            }
            for (int i = 1; i < fhh.height - 1; i++) {
                for (int j = 1; j < fhh.width - 1; j++) {
                    pic[i][j].I = pic[i][j].tI;
                }
            }
            HSI_u_RGB();
            break;
        case 25:
            RGB_u_HSI();
            for (int i = 1; i < fhh.height - 1; i++) {
                for (int j = 1; j < fhh.width - 1; j++) {
                    if (pic[i - 1][j].I || pic[i][j - 1].I || pic[i][j].I || pic[i][j + 1].I || pic[i + 1][j].I)
                        pic[i][j].tI = 1;
                    else
                        pic[i][j].tI = 0;
                }
            }
            for (int i = 1; i < fhh.height - 1; i++) {
                for (int j = 1; j < fhh.width - 1; j++) {
                    pic[i][j].I = pic[i][j].tI;
                }
            }
            HSI_u_RGB();
            break;
        case 26:
            erozija();
            dilatacija();
            break;
        case 27:
            dilatacija();
            erozija();
            break;
        default:
            break;
    }

    fp1 = fopen(xy, "wb");

    fwrite(&mag, sizeof(mag), 1, fp1);
    fwrite(&fh, sizeof(fh), 1, fp1);
    fwrite(&fhh, sizeof(fhh), 1, fp1);
    for (int i = 0; i < fhh.height; i++) {
        for (int j = 0; j < fhh.width; j++)
            fwrite(&pic[i][j], 3, 1, fp1);
        fwrite(&pp, pad, 1, fp1);
    }

    fclose(fp1);

    slika2->setPixmap(QPixmap("privremeno.bmp"));

    crtanje_histograma(2);
}

void GlavnaStrana::RGB_u_HSI() {
    float R, G, B, r, g, b, w, in;
    for(int k = 0; k < 256; k++) {
        gustina_raspodele[k] = 0;
    }
    for (int i = 0; i < fhh.height; i++) {
        for (int j = 0; j < fhh.width; j++) {
            R = pic[i][j].R / 255.0, G = pic[i][j].G / 255.0, B = pic[i][j].B / 255.0;
            pic[i][j].I = (in = R + G + B) / 3.0;
            r = R / in;
            g = G / in;
            b = B / in;
            if(R == G && G == B) {
                pic[i][j].S = 0;
                pic[i][j].H = 0;
            }
            else {
                w = 0.5 * (R - G + R - B) / sqrt((R - G) * (R - G) + (R - B) * (G - B));
                if(w > 1)
                    w = 1;
                if(w < -1)
                    w = -1;
                pic[i][j].H = acos(w);
                if(B > G)
                    pic[i][j].H = 2 * M_PI - pic[i][j].H;
                if(r <= g && r <= b)
                    pic[i][j].S = 1 - 3 * r;
                if(g <= r && g <= b)
                    pic[i][j].S = 1 - 3 * g;
                if(b <= r && b <= g)
                    pic[i][j].S = 1 - 3 * b;
            }
            gustina_raspodele[(int)round(pic[i][j].I * 255)]++;
        }
    }
    maksimalna_gustina = 0;
    for(int k = 0; k < 256; k++) {
        gustina_raspodele[k] /= fhh.height * fhh.width;
        if(gustina_raspodele[k] > maksimalna_gustina)
            maksimalna_gustina = gustina_raspodele[k];
    }
}

void GlavnaStrana::HSI_u_RGB() {
    float H, S, I, R, G, B, r, g, b;
    for (int i = 0; i < fhh.height; i++) {
        for (int j = 0; j < fhh.width; j++) {
            H = pic[i][j].H;
            S = pic[i][j].S;
            I = pic[i][j].I;
            if(S > 1)
                S = 1;
            if(I > 1)
                I = 1;
            if(S == 0)
                pic[i][j].R = pic[i][j].G = pic[i][j].B = (int)round(I * 255);
            else {
                if(H < 2 * M_PI / 3) {
                    b = (1 - S) / 3;
                    r = (1 + S * cos(H) / cos(M_PI / 3 - H)) / 3;
                    g = 1 - r - b;
                }
                else if(H < 4 * M_PI / 3) {
                    H = H - 2 * M_PI / 3;
                    r = (1 - S) / 3;
                    g = (1 + S * cos(H) / cos(M_PI / 3 - H)) / 3;
                    b = 1 - r - g;
                }
                else {
                    H = H - 4 * M_PI / 3;
                    g = (1 - S) / 3;
                    b = (1 + S * cos(H) / cos(M_PI / 3 - H)) / 3;
                    r = 1 - b - g;
                }
                if(r < 0)
                    r = 0;
                if(g < 0)
                    g = 0;
                if(b < 0)
                    b = 0;
                R = 3 * I * r;
                G = 3 * I * g;
                B = 3 * I * b;
                if(R > 1)
                    R = 1;
                if(G > 1)
                    G = 1;
                if(B > 1)
                    B = 1;
                pic[i][j].R = (int)round(R * 255);
                pic[i][j].G = (int)round(G * 255);
                pic[i][j].B = (int)round(B * 255);
            }
        }
    }
}

void GlavnaStrana::ekvalizacija() {
    for (int k = 0; k < 256; k++) {
        funkcija_raspodele[k] = 0;
    }
    for (int k = 0; k < 256; k++) {
        for(int l = 0; l < k; l++) {
            funkcija_raspodele[k] += gustina_raspodele[l];
        }
    }
    for (int i = 0; i < fhh.height; i++) {
        for (int j = 0; j < fhh.width; j++) {
            pic[i][j].I = funkcija_raspodele[(int)round(pic[i][j].I * 255)];
        }
    }
}

void GlavnaStrana::crtanje_histograma(int arg) {
    RGB_u_HSI();
    for(int k = 0; k < 255; k++) {
        if(arg == 1) {
            histogram1[k].setP2(QPoint(histogram1[k].p2().x(), (260 - gustina_raspodele[k] * 250 / maksimalna_gustina)));
            sacuvan_histogram1[k]->setLine(histogram1[k]);
        }
        else {
            histogram2[k].setP2(QPoint(histogram2[k].p2().x(), (522 - gustina_raspodele[k] * 250 / maksimalna_gustina)));
            sacuvan_histogram2[k]->setLine(histogram2[k]);
        }
    }
}

void GlavnaStrana::uprosecavanje_f() {
    for (int i = 1; i < fhh.height - 1; i++) {
        for (int j = 1; j < fhh.width - 1; j++) {
            pic[i][j].tI = (pic[i - 1][j - 1].I + pic[i - 1][j].I + pic[i - 1][j + 1].I + pic[i][j - 1].I + pic[i][j].I +
                    pic[i][j + 1].I + pic[i + 1][j - 1].I + pic[i + 1][j].I + pic[i + 1][j + 1].I) / 9.0;
        }
    }
}

void GlavnaStrana::medijana_f() {
    for (int i = 1; i < fhh.height - 1; i++) {
        for (int j = 1; j < fhh.width - 1; j++) {
            int n1[9] = {(int)round(pic[i - 1][j - 1].I * 255), (int)round(pic[i - 1][j].I * 255),
                         (int)round(pic[i - 1][j + 1].I * 255), (int)round(pic[i][j - 1].I * 255),
                         (int)round(pic[i][j].I * 255), (int)round(pic[i][j + 1].I * 255), (int)round(pic[i + 1][j - 1].I * 255),
                         (int)round(pic[i + 1][j].I * 255), (int)round(pic[i + 1][j + 1].I * 255)};
            qsort(n1, 9, sizeof(int), poredjenje);
            pic[i][j].tI = n1[5] / 255.0;
        }
    }
}

void GlavnaStrana::laplasijan_filter() {
    for (int i = 1; i < fhh.height - 1; i++) {
        for (int j = 1; j < fhh.width - 1; j++) {
            pic[i][j].tI = (pic[i - 1][j].I + pic[i][j - 1].I - 5 * pic[i][j].I + pic[i][j + 1].I + pic[i + 1][j].I) / (-1.0);
        }
    }
}

void GlavnaStrana::spacijalni_u_frekventni(int i, int j, int Q50[8][8], double D_Y[8][8], double D_Cb[8][8], double D_Cr[8][8],
                                           double M_Y[8][8], double M_Cb[8][8], double M_Cr[8][8], double T[8][8]) {
    for (int k = i; k < i + 8; k++) {
        for (int l = j; l < j + 8; l++) {
            M_Y[k - i][l - j] = pic[k][l].Y - 128;
            M_Cb[k - i][l - j] = pic[k][l].CB - 128;
            M_Cr[k - i][l - j] = pic[k][l].CR - 128;
        }
    }
    for (int k = i; k < i + 8; k++) {
        for (int l = j; l < j + 8; l++) {
            D_Y[k - i][l - j] = 0;
            D_Cb[k - i][l - j] = 0;
            D_Cr[k - i][l - j] = 0;
            for (int m = 0; m < 8; m++) {
                D_Y[k - i][l - j] += T[k - i][m] * M_Y[m][l - j];
                D_Cb[k - i][l - j] += T[k - i][m] * M_Cb[m][l - j];
                D_Cr[k - i][l - j] += T[k - i][m] * M_Cr[m][l - j];
            }
        }
    }
    for (int k = i; k < i + 8; k++) {
        for (int l = j; l < j + 8; l++) {
            pic[k][l].F_Y = 0.0;
            pic[k][l].F_Cb = 0.0;
            pic[k][l].F_Cr = 0.0;
            for (int m = 0; m < 8; m++) {
                pic[k][l].F_Y += D_Y[k - i][m] * T[l - j][m];
                pic[k][l].F_Cb += D_Cb[k - i][m] * T[l - j][m];
                pic[k][l].F_Cr += D_Cr[k - i][m] * T[l - j][m];
            }
        }
    }
}

void GlavnaStrana::kvantizacija(int i, int j, int Q50[8][8], double D_Y[8][8], double D_Cb[8][8], double D_Cr[8][8],
                                double M_Y[8][8], double M_Cb[8][8], double M_Cr[8][8], double T[8][8]) {
    double faktor_kompresije = kompresija_slajder->value() / 2/*!*/ + 5;
    faktor_kompresije = faktor_kompresije > 50 ? (100 - faktor_kompresije) / 50.0 : 50.0 / faktor_kompresije;
    for (int k = i; k < i + 8; k++) {
        for (int l = j; l < j + 8; l++) {
            pic[k][l].F_Y = round(pic[k][l].F_Y / (Q50[k - i][l - j] * faktor_kompresije));
            pic[k][l].F_Cb = round(pic[k][l].F_Cb / (Q50[k - i][l - j] * faktor_kompresije));
            pic[k][l].F_Cr = round(pic[k][l].F_Cr / (Q50[k - i][l - j] * faktor_kompresije));
        }
    }
    for (int k = i; k < i + 8; k++) {
        for (int l = j; l < j + 8; l++) {
            pic[k][l].F_Y *= Q50[k - i][l - j] * faktor_kompresije;
            pic[k][l].F_Cb *= Q50[k - i][l - j] * faktor_kompresije;
            pic[k][l].F_Cr *= Q50[k - i][l - j] * faktor_kompresije;
        }
    }
}

void GlavnaStrana::frekventni_u_spacijalni(int i, int j, int Q50[8][8], double D_Y[8][8], double D_Cb[8][8], double D_Cr[8][8],
                                           double M_Y[8][8], double M_Cb[8][8], double M_Cr[8][8], double T[8][8]) {
    for (int k = i; k < i + 8; k++) {
        for (int l = j; l < j + 8; l++) {
            D_Y[k - i][l - j] = 0;
            D_Cb[k - i][l - j] = 0;
            D_Cr[k - i][l - j] = 0;
            for (int m = 0; m < 8; m++) {
                D_Y[k - i][l - j] += T[m][k - i] * pic[i + m][l].F_Y;
                D_Cb[k - i][l - j] += T[m][k - i] * pic[i + m][l].F_Cb;
                D_Cr[k - i][l - j] += T[m][k - i] * pic[i + m][l].F_Cr;
            }
        }
    }
    for (int k = i; k < i + 8; k++) {
        for (int l = j; l < j + 8; l++) {
            pic[k][l].Y = 128;
            pic[k][l].CB = 128;
            pic[k][l].CR = 128;
            double suma_Y = 0.0, suma_Cb = 0.0, suma_Cr = 0.0;
            for (int m = 0; m < 8; m++) {
                suma_Y += D_Y[k - i][m] * T[m][l - j];
                suma_Cb += D_Cb[k - i][m] * T[m][l - j];
                suma_Cr += D_Cr[k - i][m] * T[m][l - j];
            }
            pic[k][l].Y += round(suma_Y);
            pic[k][l].CB += round(suma_Cb);
            pic[k][l].CR += round(suma_Cr);
        }
    }
}

void GlavnaStrana::cuvanje() {
    if(otvaranjeFajlaZaPisanje() == 0)
        return;

    fp2 = fopen(yy, "wb");

    fwrite(&mag, sizeof(mag), 1, fp2);
    fwrite(&fh, sizeof(fh), 1, fp2);
    fwrite(&fhh, sizeof(fhh), 1, fp2);
    for (int i = 0; i < fhh.height; i++) {
        for (int j = 0; j < fhh.width; j++)
            fwrite(&pic[i][j], 3, 1, fp2);
        fwrite(&pp, pad, 1, fp2);
    }

    zatvaranje_fajlova();
}

void GlavnaStrana::zatvaranje_fajlova() {
    if(fp)
        fclose(fp);
    if(fp2)
        fclose(fp2);
}

int GlavnaStrana::otvaranjeFajlaZaCitanje() {
    ucitanFajl = QFileDialog::getOpenFileName(this, tr("Izaberi sliku"), "Slike/", tr("Slike (*.bmp)"));
    xx = ucitanFajl.toLatin1().data();
    if (ucitanFajl.isNull())
        return 0;
    return 1;
}

int GlavnaStrana::otvaranjeFajlaZaPisanje() {
    fajlZaCuvanje = QFileDialog::getSaveFileName(this, tr("Izaberi sliku"), "", tr("Slike (*.bmp)"));
    yy = fajlZaCuvanje.toLatin1().data();
    if (fajlZaCuvanje.isNull())
        return 0;
    return 1;
}
