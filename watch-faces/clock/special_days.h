#include "special_day_face.h"
#define SPECIAL_DAYS_START_DATE_YEAR 2026
#define SPECIAL_DAYS_START_DATE_MONTH 1
#define SPECIAL_DAYS_START_DATE_DAY 17

static const char text_AllSaints[] = "AllSaints";
static const char text_AllSouls[] = "AllSouls";
static const char text_Annunciation[] = "Annunciation";
static const char text_Ascension[] = "Ascension";
static const char text_AshWednesday[] = "AshWednesday";
static const char text_Assumption[] = "Assumption";
static const char text_BaptismLord[] = "BaptismLord";
static const char text_BeheadingJohnBaptist[] = "BeheadingJohnBaptist";
static const char text_ChairStPeter[] = "ChairStPeter";
static const char text_ChristKing[] = "ChristKing";
static const char text_Christmas[] = "Christmas";
static const char text_ConversionStPaul[] = "ConversionStPaul";
static const char text_CorpusChristi[] = "CorpusChristi";
static const char text_DedicationLateran[] = "DedicationLateran";
static const char text_DedicationStMaryMajor[] = "DedicationStMaryMajor";
static const char text_DedicationStsPeterPaul[] = "DedicationStsPeterPaul";
static const char text_Easter[] = "Easter";
static const char text_EasterVigil[] = "EasterVigil";
static const char text_Epiphany[] = "Epiphany";
static const char text_ExaltationCross[] = "ExaltationCross";
static const char text_FirstMartyrsRome[] = "FirstMartyrsRome";
static const char text_GoodFri[] = "GoodFri";
static const char text_GuardianAngels[] = "GuardianAngels";
static const char text_HolyFamily[] = "HolyFamily";
static const char text_HolyInnocents[] = "HolyInnocents";
static const char text_HolyNameMary[] = "HolyNameMary";
static const char text_HolyThursChrism[] = "HolyThursChrism";
static const char text_ImmaculateConception[] = "ImmaculateConception";
static const char text_ImmaculateHeart[] = "ImmaculateHeart";
static const char text_JuanDiego[] = "JuanDiego";
static const char text_MaryMotherChurch[] = "MaryMotherChurch";
static const char text_MaryMotherOfGod[] = "MaryMotherOfGod";
static const char text_NameJesus[] = "NameJesus";
static const char text_NativityJohnBaptist[] = "NativityJohnBaptist";
static const char text_NativityVirginMary[] = "NativityVirginMary";
static const char text_OurLadyOfFatima[] = "OurLadyOfFatima";
static const char text_OurLadyOfGuadalupe[] = "OurLadyOfGuadalupe";
static const char text_OurLadyOfLoreto[] = "OurLadyOfLoreto";
static const char text_OurLadyOfLourdes[] = "OurLadyOfLourdes";
static const char text_OurLadyOfMountCarmel[] = "OurLadyOfMountCarmel";
static const char text_OurLadyOfSorrows[] = "OurLadyOfSorrows";
static const char text_OurLadyOfTheRosary[] = "OurLadyOfTheRosary";
static const char text_PalmSun[] = "PalmSun";
static const char text_Pentecost[] = "Pentecost";
static const char text_Presentation[] = "Presentation";
static const char text_PresentationMary[] = "PresentationMary";
static const char text_QueenshipMary[] = "QueenshipMary";
static const char text_SacredHeart[] = "SacredHeart";
static const char text_SevenHolyFounders[] = "SevenHolyFounders";
static const char text_StAgatha[] = "StAgatha";
static const char text_StAgnes[] = "StAgnes";
static const char text_StAlbertGreat[] = "StAlbertGreat";
static const char text_StAloysiusGonzaga[] = "StAloysiusGonzaga";
static const char text_StAlphonsusMariaDeLiguori[] = "StAlphonsusMariaDeLiguori";
static const char text_StAmbrose[] = "StAmbrose";
static const char text_StAndrewAp[] = "StAndrewAp";
static const char text_StAndrewDungLac[] = "StAndrewDungLac";
static const char text_StAndrewKimTaegon[] = "StAndrewKimTaegon";
static const char text_StAngelaMerici[] = "StAngelaMerici";
static const char text_StAnselm[] = "StAnselm";
static const char text_StAnthonyEgypt[] = "StAnthonyEgypt";
static const char text_StAnthonyMaryClaret[] = "StAnthonyMaryClaret";
static const char text_StAnthonyPadua[] = "StAnthonyPadua";
static const char text_StAnthonyZaccaria[] = "StAnthonyZaccaria";
static const char text_StApollinaris[] = "StApollinaris";
static const char text_StAthanasius[] = "StAthanasius";
static const char text_StAugustineCanterbury[] = "StAugustineCanterbury";
static const char text_StAugustineHippo[] = "StAugustineHippo";
static const char text_StAugustineZhaoRong[] = "StAugustineZhaoRong";
static const char text_StBarnabasAp[] = "StBarnabasAp";
static const char text_StBartholomewAp[] = "StBartholomewAp";
static const char text_StBedeVenerable[] = "StBedeVenerable";
static const char text_StBenedict[] = "StBenedict";
static const char text_StBernardClairvaux[] = "StBernardClairvaux";
static const char text_StBernardineSiena[] = "StBernardineSiena";
static const char text_StBlase[] = "StBlase";
static const char text_StBonaventure[] = "StBonaventure";
static const char text_StBoniface[] = "StBoniface";
static const char text_StBridget[] = "StBridget";
static const char text_StBruno[] = "StBruno";
static const char text_StCallistusIPope[] = "StCallistusIPope";
static const char text_StCamillusDeLellis[] = "StCamillusDeLellis";
static const char text_StCasimir[] = "StCasimir";
static const char text_StCatherineAlexandria[] = "StCatherineAlexandria";
static const char text_StCatherineSiena[] = "StCatherineSiena";
static const char text_StCecilia[] = "StCecilia";
static const char text_StCharlesBorromeo[] = "StCharlesBorromeo";
static const char text_StChristopherMagallanes[] = "StChristopherMagallanes";
static const char text_StClare[] = "StClare";
static const char text_StClementIPope[] = "StClementIPope";
static const char text_StCyrilAlexandria[] = "StCyrilAlexandria";
static const char text_StCyrilJerusalem[] = "StCyrilJerusalem";
static const char text_StDamasusIPope[] = "StDamasusIPope";
static const char text_StDenis[] = "StDenis";
static const char text_StDominic[] = "StDominic";
static const char text_StEdithStein[] = "StEdithStein";
static const char text_StElizabethHungary[] = "StElizabethHungary";
static const char text_StElizabethPortugal[] = "StElizabethPortugal";
static const char text_StEphrem[] = "StEphrem";
static const char text_StEusebius[] = "StEusebius";
static const char text_StFabianPope[] = "StFabianPope";
static const char text_StFaustinaKowalska[] = "StFaustinaKowalska";
static const char text_StFidelisSigmaringen[] = "StFidelisSigmaringen";
static const char text_StFrancesRome[] = "StFrancesRome";
static const char text_StFrancisAssisi[] = "StFrancisAssisi";
static const char text_StFrancisDeSales[] = "StFrancisDeSales";
static const char text_StFrancisPaola[] = "StFrancisPaola";
static const char text_StFrancisXavier[] = "StFrancisXavier";
static const char text_StGeorge[] = "StGeorge";
static const char text_StGregoryGreat[] = "StGregoryGreat";
static const char text_StGregoryNarek[] = "StGregoryNarek";
static const char text_StHedwig[] = "StHedwig";
static const char text_StHenry[] = "StHenry";
static const char text_StHilaryPoitiers[] = "StHilaryPoitiers";
static const char text_StIgnatiusAntioch[] = "StIgnatiusAntioch";
static const char text_StIgnatiusLoyola[] = "StIgnatiusLoyola";
static const char text_StIrenaeus[] = "StIrenaeus";
static const char text_StIsidore[] = "StIsidore";
static const char text_StJamesAp[] = "StJamesAp";
static const char text_StJaneFrancesDeChantal[] = "StJaneFrancesDeChantal";
static const char text_StJanuarius[] = "StJanuarius";
static const char text_StJeanVianney[] = "StJeanVianney";
static const char text_StJerome[] = "StJerome";
static const char text_StJeromeEmiliani[] = "StJeromeEmiliani";
static const char text_StJohnAvila[] = "StJohnAvila";
static const char text_StJohnBaptistDeLaSalle[] = "StJohnBaptistDeLaSalle";
static const char text_StJohnBosco[] = "StJohnBosco";
static const char text_StJohnCapistrano[] = "StJohnCapistrano";
static const char text_StJohnChrysostom[] = "StJohnChrysostom";
static const char text_StJohnCross[] = "StJohnCross";
static const char text_StJohnDamascene[] = "StJohnDamascene";
static const char text_StJohnEudes[] = "StJohnEudes";
static const char text_StJohnEvangelist[] = "StJohnEvangelist";
static const char text_StJohnGod[] = "StJohnGod";
static const char text_StJohnIPope[] = "StJohnIPope";
static const char text_StJohnKanty[] = "StJohnKanty";
static const char text_StJohnPaulII[] = "StJohnPaulII";
static const char text_StJohnXXIII[] = "StJohnXXIII";
static const char text_StJosaphat[] = "StJosaphat";
static const char text_StJoseph[] = "StJoseph";
static const char text_StJosephWorker[] = "StJosephWorker";
static const char text_StJustinMartyr[] = "StJustinMartyr";
static const char text_StLawrenceBrindisi[] = "StLawrenceBrindisi";
static const char text_StLawrenceDeacon[] = "StLawrenceDeacon";
static const char text_StLeoGreat[] = "StLeoGreat";
static const char text_StLouis[] = "StLouis";
static const char text_StLucySyracuse[] = "StLucySyracuse";
static const char text_StLukeEvangelist[] = "StLukeEvangelist";
static const char text_StMargaretScotland[] = "StMargaretScotland";
static const char text_StMariaGoretti[] = "StMariaGoretti";
static const char text_StMarkEvangelist[] = "StMarkEvangelist";
static const char text_StMartha[] = "StMartha";
static const char text_StMartinPope[] = "StMartinPope";
static const char text_StMartinPorres[] = "StMartinPorres";
static const char text_StMartinTours[] = "StMartinTours";
static const char text_StMaryMagdalene[] = "StMaryMagdalene";
static const char text_StMatthewEvangelist[] = "StMatthewEvangelist";
static const char text_StMatthiasAp[] = "StMatthiasAp";
static const char text_StMaximilianKolbe[] = "StMaximilianKolbe";
static const char text_StMonica[] = "StMonica";
static const char text_StMotherTeresa[] = "StMotherTeresa";
static const char text_StNicholas[] = "StNicholas";
static const char text_StNorbert[] = "StNorbert";
static const char text_StPatrick[] = "StPatrick";
static const char text_StPaulVI[] = "StPaulVI";
static const char text_StPaulinusNola[] = "StPaulinusNola";
static const char text_StPeterCanisius[] = "StPeterCanisius";
static const char text_StPeterChanel[] = "StPeterChanel";
static const char text_StPeterChrysologus[] = "StPeterChrysologus";
static const char text_StPeterClaver[] = "StPeterClaver";
static const char text_StPeterDamian[] = "StPeterDamian";
static const char text_StPhilipNeri[] = "StPhilipNeri";
static const char text_StPioPietrelcina[] = "StPioPietrelcina";
static const char text_StPiusV[] = "StPiusV";
static const char text_StPiusX[] = "StPiusX";
static const char text_StPolycarp[] = "StPolycarp";
static const char text_StRayPenyafort[] = "StRayPenyafort";
static const char text_StRitaCascia[] = "StRitaCascia";
static const char text_StRobertBellarmine[] = "StRobertBellarmine";
static const char text_StRomuald[] = "StRomuald";
static const char text_StRoseLima[] = "StRoseLima";
static const char text_StScholastica[] = "StScholastica";
static const char text_StSharbelMakhluf[] = "StSharbelMakhluf";
static const char text_StSimonStJudeAp[] = "StSimonStJudeAp";
static const char text_StSixtusIIPope[] = "StSixtusIIPope";
static const char text_StStanislaus[] = "StStanislaus";
static const char text_StStephenHungary[] = "StStephenHungary";
static const char text_StStephenProtomartyr[] = "StStephenProtomartyr";
static const char text_StSylvesterIPope[] = "StSylvesterIPope";
static const char text_StTeresaJesus[] = "StTeresaJesus";
static const char text_StThereseChildJesus[] = "StThereseChildJesus";
static const char text_StThomasAp[] = "StThomasAp";
static const char text_StThomasAquinas[] = "StThomasAquinas";
static const char text_StThomasBecket[] = "StThomasBecket";
static const char text_StTuribius[] = "StTuribius";
static const char text_StVincentDePaul[] = "StVincentDePaul";
static const char text_StVincentDeacon[] = "StVincentDeacon";
static const char text_StVincentFerrer[] = "StVincentFerrer";
static const char text_StWenceslaus[] = "StWenceslaus";
static const char text_StsArchangels[] = "StsArchangels";
static const char text_StsBasilGreg[] = "StsBasilGreg";
static const char text_StsCharlesLwanga[] = "StsCharlesLwanga";
static const char text_StsCorneliusCyprian[] = "StsCorneliusCyprian";
static const char text_StsCosmasDamian[] = "StsCosmasDamian";
static const char text_StsCyrilMethodius[] = "StsCyrilMethodius";
static const char text_StsJeanBrebeuf[] = "StsJeanBrebeuf";
static const char text_StsJoachimAnne[] = "StsJoachimAnne";
static const char text_StsMarcellinusPeter[] = "StsMarcellinusPeter";
static const char text_StsNereusAchilleus[] = "StsNereusAchilleus";
static const char text_StsPaulMiki[] = "StsPaulMiki";
static const char text_StsPerpetuaFelicity[] = "StsPerpetuaFelicity";
static const char text_StsPeterPaulAp[] = "StsPeterPaulAp";
static const char text_StsPhilipJames[] = "StsPhilipJames";
static const char text_StsPontianHippolytus[] = "StsPontianHippolytus";
static const char text_StsTimothyTitus[] = "StsTimothyTitus";
static const char text_Transfiguration[] = "Transfiguration";
static const char text_Trinity[] = "Trinity";
static const char text_Visitation[] = "Visitation";

static const SpecialDay special_days[] = {
    /* 000 2026-01-17 */ { ORDINARY_TIME, 1, false, false, text_StAnthonyEgypt },
    /* 001 2026-01-18 */ { ORDINARY_TIME, 2, true, false, NULL },
    /* 002 2026-01-19 */ { ORDINARY_TIME, 2, false, false, NULL },
    /* 003 2026-01-20 */ { ORDINARY_TIME, 2, false, false, text_StFabianPope },
    /* 004 2026-01-21 */ { ORDINARY_TIME, 2, false, false, text_StAgnes },
    /* 005 2026-01-22 */ { ORDINARY_TIME, 2, false, false, text_StVincentDeacon },
    /* 006 2026-01-23 */ { ORDINARY_TIME, 2, false, true, NULL },
    /* 007 2026-01-24 */ { ORDINARY_TIME, 2, false, false, text_StFrancisDeSales },
    /* 008 2026-01-25 */ { ORDINARY_TIME, 3, true, false, text_ConversionStPaul },
    /* 009 2026-01-26 */ { ORDINARY_TIME, 3, false, false, text_StsTimothyTitus },
    /* 010 2026-01-27 */ { ORDINARY_TIME, 3, false, false, text_StAngelaMerici },
    /* 011 2026-01-28 */ { ORDINARY_TIME, 3, false, false, text_StThomasAquinas },
    /* 012 2026-01-29 */ { ORDINARY_TIME, 3, false, false, NULL },
    /* 013 2026-01-30 */ { ORDINARY_TIME, 3, false, true, NULL },
    /* 014 2026-01-31 */ { ORDINARY_TIME, 3, false, false, text_StJohnBosco },
    /* 015 2026-02-01 */ { ORDINARY_TIME, 4, true, false, NULL },
    /* 016 2026-02-02 */ { ORDINARY_TIME, 4, false, false, text_Presentation },
    /* 017 2026-02-03 */ { ORDINARY_TIME, 4, false, false, text_StBlase },
    /* 018 2026-02-04 */ { ORDINARY_TIME, 4, false, false, NULL },
    /* 019 2026-02-05 */ { ORDINARY_TIME, 4, false, false, text_StAgatha },
    /* 020 2026-02-06 */ { ORDINARY_TIME, 4, false, true, text_StsPaulMiki },
    /* 021 2026-02-07 */ { ORDINARY_TIME, 4, false, false, NULL },
    /* 022 2026-02-08 */ { ORDINARY_TIME, 5, true, false, text_StJeromeEmiliani },
    /* 023 2026-02-09 */ { ORDINARY_TIME, 5, false, false, NULL },
    /* 024 2026-02-10 */ { ORDINARY_TIME, 5, false, false, text_StScholastica },
    /* 025 2026-02-11 */ { ORDINARY_TIME, 5, false, false, text_OurLadyOfLourdes },
    /* 026 2026-02-12 */ { ORDINARY_TIME, 5, false, false, NULL },
    /* 027 2026-02-13 */ { ORDINARY_TIME, 5, false, true, NULL },
    /* 028 2026-02-14 */ { ORDINARY_TIME, 5, false, false, text_StsCyrilMethodius },
    /* 029 2026-02-15 */ { ORDINARY_TIME, 6, true, false, NULL },
    /* 030 2026-02-16 */ { ORDINARY_TIME, 6, false, false, NULL },
    /* 031 2026-02-17 */ { ORDINARY_TIME, 6, false, false, text_SevenHolyFounders },
    /* 032 2026-02-18 */ { LENT, 1, false, true, text_AshWednesday },
    /* 033 2026-02-19 */ { LENT, 1, false, true, NULL },
    /* 034 2026-02-20 */ { LENT, 1, false, true, NULL },
    /* 035 2026-02-21 */ { LENT, 1, false, true, text_StPeterDamian },
    /* 036 2026-02-22 */ { LENT, 1, true, true, text_ChairStPeter },
    /* 037 2026-02-23 */ { LENT, 1, false, true, text_StPolycarp },
    /* 038 2026-02-24 */ { LENT, 1, false, true, NULL },
    /* 039 2026-02-25 */ { LENT, 1, false, true, NULL },
    /* 040 2026-02-26 */ { LENT, 1, false, true, NULL },
    /* 041 2026-02-27 */ { LENT, 1, false, true, text_StGregoryNarek },
    /* 042 2026-02-28 */ { LENT, 1, false, true, NULL },
    /* 043 2026-03-01 */ { LENT, 2, true, true, NULL },
    /* 044 2026-03-02 */ { LENT, 2, false, true, NULL },
    /* 045 2026-03-03 */ { LENT, 2, false, true, NULL },
    /* 046 2026-03-04 */ { LENT, 2, false, true, text_StCasimir },
    /* 047 2026-03-05 */ { LENT, 2, false, true, NULL },
    /* 048 2026-03-06 */ { LENT, 2, false, true, NULL },
    /* 049 2026-03-07 */ { LENT, 2, false, true, text_StsPerpetuaFelicity },
    /* 050 2026-03-08 */ { LENT, 3, true, true, text_StJohnGod },
    /* 051 2026-03-09 */ { LENT, 3, false, true, text_StFrancesRome },
    /* 052 2026-03-10 */ { LENT, 3, false, true, NULL },
    /* 053 2026-03-11 */ { LENT, 3, false, true, NULL },
    /* 054 2026-03-12 */ { LENT, 3, false, true, NULL },
    /* 055 2026-03-13 */ { LENT, 3, false, true, NULL },
    /* 056 2026-03-14 */ { LENT, 3, false, true, NULL },
    /* 057 2026-03-15 */ { LENT, 4, true, true, NULL },
    /* 058 2026-03-16 */ { LENT, 4, false, true, NULL },
    /* 059 2026-03-17 */ { LENT, 4, false, true, text_StPatrick },
    /* 060 2026-03-18 */ { LENT, 4, false, true, text_StCyrilJerusalem },
    /* 061 2026-03-19 */ { LENT, 4, true, true, text_StJoseph },
    /* 062 2026-03-20 */ { LENT, 4, false, true, NULL },
    /* 063 2026-03-21 */ { LENT, 4, false, true, NULL },
    /* 064 2026-03-22 */ { LENT, 5, true, true, NULL },
    /* 065 2026-03-23 */ { LENT, 5, false, true, text_StTuribius },
    /* 066 2026-03-24 */ { LENT, 5, false, true, NULL },
    /* 067 2026-03-25 */ { LENT, 5, false, true, text_Annunciation },
    /* 068 2026-03-26 */ { LENT, 5, false, true, NULL },
    /* 069 2026-03-27 */ { LENT, 5, false, true, NULL },
    /* 070 2026-03-28 */ { LENT, 5, false, true, NULL },
    /* 071 2026-03-29 */ { LENT, 6, true, true, text_PalmSun },
    /* 072 2026-03-30 */ { LENT, 6, false, true, NULL },
    /* 073 2026-03-31 */ { LENT, 6, false, true, NULL },
    /* 074 2026-04-01 */ { LENT, 6, false, true, NULL },
    /* 075 2026-04-02 */ { LENT, 6, false, true, text_HolyThursChrism },
    /* 076 2026-04-03 */ { EASTER_TRIDUUM, 1, false, true, text_GoodFri },
    /* 077 2026-04-04 */ { EASTER_TRIDUUM, 1, false, false, text_EasterVigil },
    /* 078 2026-04-05 */ { EASTER_TRIDUUM, 1, true, false, text_Easter },
    /* 079 2026-04-06 */ { EASTER, 1, false, false, NULL },
    /* 080 2026-04-07 */ { EASTER, 1, false, false, text_StJohnBaptistDeLaSalle },
    /* 081 2026-04-08 */ { EASTER, 1, false, false, NULL },
    /* 082 2026-04-09 */ { EASTER, 1, false, false, NULL },
    /* 083 2026-04-10 */ { EASTER, 1, false, true, NULL },
    /* 084 2026-04-11 */ { EASTER, 1, false, false, text_StStanislaus },
    /* 085 2026-04-12 */ { EASTER, 2, true, false, NULL },
    /* 086 2026-04-13 */ { EASTER, 2, false, false, text_StMartinPope },
    /* 087 2026-04-14 */ { EASTER, 2, false, false, NULL },
    /* 088 2026-04-15 */ { EASTER, 2, false, false, NULL },
    /* 089 2026-04-16 */ { EASTER, 2, false, false, NULL },
    /* 090 2026-04-17 */ { EASTER, 2, false, true, NULL },
    /* 091 2026-04-18 */ { EASTER, 2, false, false, NULL },
    /* 092 2026-04-19 */ { EASTER, 3, true, false, NULL },
    /* 093 2026-04-20 */ { EASTER, 3, false, false, NULL },
    /* 094 2026-04-21 */ { EASTER, 3, false, false, text_StAnselm },
    /* 095 2026-04-22 */ { EASTER, 3, false, false, NULL },
    /* 096 2026-04-23 */ { EASTER, 3, false, false, text_StGeorge },
    /* 097 2026-04-24 */ { EASTER, 3, false, true, text_StFidelisSigmaringen },
    /* 098 2026-04-25 */ { EASTER, 3, false, false, text_StMarkEvangelist },
    /* 099 2026-04-26 */ { EASTER, 4, true, false, NULL },
    /* 100 2026-04-27 */ { EASTER, 4, false, false, NULL },
    /* 101 2026-04-28 */ { EASTER, 4, false, false, text_StPeterChanel },
    /* 102 2026-04-29 */ { EASTER, 4, false, false, text_StCatherineSiena },
    /* 103 2026-04-30 */ { EASTER, 4, false, false, text_StPiusV },
    /* 104 2026-05-01 */ { EASTER, 4, false, true, text_StJosephWorker },
    /* 105 2026-05-02 */ { EASTER, 4, false, false, text_StAthanasius },
    /* 106 2026-05-03 */ { EASTER, 5, true, false, text_StsPhilipJames },
    /* 107 2026-05-04 */ { EASTER, 5, false, false, NULL },
    /* 108 2026-05-05 */ { EASTER, 5, false, false, NULL },
    /* 109 2026-05-06 */ { EASTER, 5, false, false, NULL },
    /* 110 2026-05-07 */ { EASTER, 5, false, false, NULL },
    /* 111 2026-05-08 */ { EASTER, 5, false, true, NULL },
    /* 112 2026-05-09 */ { EASTER, 5, false, false, NULL },
    /* 113 2026-05-10 */ { EASTER, 6, true, false, NULL },
    /* 114 2026-05-11 */ { EASTER, 6, false, false, NULL },
    /* 115 2026-05-12 */ { EASTER, 6, false, false, text_StsNereusAchilleus },
    /* 116 2026-05-13 */ { EASTER, 6, false, false, text_OurLadyOfFatima },
    /* 117 2026-05-14 */ { EASTER, 6, true, false, text_Ascension },
    /* 118 2026-05-15 */ { EASTER, 6, false, true, NULL },
    /* 119 2026-05-16 */ { EASTER, 6, false, false, NULL },
    /* 120 2026-05-17 */ { EASTER, 7, true, false, NULL },
    /* 121 2026-05-18 */ { EASTER, 7, false, false, text_StJohnIPope },
    /* 122 2026-05-19 */ { EASTER, 7, false, false, NULL },
    /* 123 2026-05-20 */ { EASTER, 7, false, false, text_StBernardineSiena },
    /* 124 2026-05-21 */ { EASTER, 7, false, false, text_StChristopherMagallanes },
    /* 125 2026-05-22 */ { EASTER, 7, false, true, text_StRitaCascia },
    /* 126 2026-05-23 */ { EASTER, 7, false, false, NULL },
    /* 127 2026-05-24 */ { EASTER, 8, true, false, text_Pentecost },
    /* 128 2026-05-25 */ { ORDINARY_TIME, 7, false, false, text_MaryMotherChurch },
    /* 129 2026-05-26 */ { ORDINARY_TIME, 7, false, false, text_StPhilipNeri },
    /* 130 2026-05-27 */ { ORDINARY_TIME, 7, false, false, text_StAugustineCanterbury },
    /* 131 2026-05-28 */ { ORDINARY_TIME, 7, false, false, NULL },
    /* 132 2026-05-29 */ { ORDINARY_TIME, 7, false, true, text_StPaulVI },
    /* 133 2026-05-30 */ { ORDINARY_TIME, 7, false, false, NULL },
    /* 134 2026-05-31 */ { ORDINARY_TIME, 8, true, false, text_Trinity },
    /* 135 2026-06-01 */ { ORDINARY_TIME, 8, false, false, text_StJustinMartyr },
    /* 136 2026-06-02 */ { ORDINARY_TIME, 8, false, false, text_StsMarcellinusPeter },
    /* 137 2026-06-03 */ { ORDINARY_TIME, 8, false, false, text_StsCharlesLwanga },
    /* 138 2026-06-04 */ { ORDINARY_TIME, 8, false, false, NULL },
    /* 139 2026-06-05 */ { ORDINARY_TIME, 8, false, true, text_StBoniface },
    /* 140 2026-06-06 */ { ORDINARY_TIME, 8, false, false, text_StNorbert },
    /* 141 2026-06-07 */ { ORDINARY_TIME, 9, true, false, text_CorpusChristi },
    /* 142 2026-06-08 */ { ORDINARY_TIME, 9, false, false, NULL },
    /* 143 2026-06-09 */ { ORDINARY_TIME, 9, false, false, text_StEphrem },
    /* 144 2026-06-10 */ { ORDINARY_TIME, 9, false, false, NULL },
    /* 145 2026-06-11 */ { ORDINARY_TIME, 9, false, false, text_StBarnabasAp },
    /* 146 2026-06-12 */ { ORDINARY_TIME, 9, false, true, text_SacredHeart },
    /* 147 2026-06-13 */ { ORDINARY_TIME, 9, false, false, text_ImmaculateHeart },
    /* 148 2026-06-14 */ { ORDINARY_TIME, 10, true, false, NULL },
    /* 149 2026-06-15 */ { ORDINARY_TIME, 10, false, false, NULL },
    /* 150 2026-06-16 */ { ORDINARY_TIME, 10, false, false, NULL },
    /* 151 2026-06-17 */ { ORDINARY_TIME, 10, false, false, NULL },
    /* 152 2026-06-18 */ { ORDINARY_TIME, 10, false, false, NULL },
    /* 153 2026-06-19 */ { ORDINARY_TIME, 10, false, true, text_StRomuald },
    /* 154 2026-06-20 */ { ORDINARY_TIME, 10, false, false, NULL },
    /* 155 2026-06-21 */ { ORDINARY_TIME, 11, true, false, text_StAloysiusGonzaga },
    /* 156 2026-06-22 */ { ORDINARY_TIME, 11, false, false, text_StPaulinusNola },
    /* 157 2026-06-23 */ { ORDINARY_TIME, 11, false, false, NULL },
    /* 158 2026-06-24 */ { ORDINARY_TIME, 11, false, false, text_NativityJohnBaptist },
    /* 159 2026-06-25 */ { ORDINARY_TIME, 11, false, false, NULL },
    /* 160 2026-06-26 */ { ORDINARY_TIME, 11, false, true, NULL },
    /* 161 2026-06-27 */ { ORDINARY_TIME, 11, false, false, text_StCyrilAlexandria },
    /* 162 2026-06-28 */ { ORDINARY_TIME, 12, true, false, text_StIrenaeus },
    /* 163 2026-06-29 */ { ORDINARY_TIME, 12, true, false, text_StsPeterPaulAp },
    /* 164 2026-06-30 */ { ORDINARY_TIME, 12, false, false, text_FirstMartyrsRome },
    /* 165 2026-07-01 */ { ORDINARY_TIME, 12, false, false, NULL },
    /* 166 2026-07-02 */ { ORDINARY_TIME, 12, false, false, NULL },
    /* 167 2026-07-03 */ { ORDINARY_TIME, 12, false, true, text_StThomasAp },
    /* 168 2026-07-04 */ { ORDINARY_TIME, 12, false, false, text_StElizabethPortugal },
    /* 169 2026-07-05 */ { ORDINARY_TIME, 13, true, false, text_StAnthonyZaccaria },
    /* 170 2026-07-06 */ { ORDINARY_TIME, 13, false, false, text_StMariaGoretti },
    /* 171 2026-07-07 */ { ORDINARY_TIME, 13, false, false, NULL },
    /* 172 2026-07-08 */ { ORDINARY_TIME, 13, false, false, NULL },
    /* 173 2026-07-09 */ { ORDINARY_TIME, 13, false, false, text_StAugustineZhaoRong },
    /* 174 2026-07-10 */ { ORDINARY_TIME, 13, false, true, NULL },
    /* 175 2026-07-11 */ { ORDINARY_TIME, 13, false, false, text_StBenedict },
    /* 176 2026-07-12 */ { ORDINARY_TIME, 14, true, false, NULL },
    /* 177 2026-07-13 */ { ORDINARY_TIME, 14, false, false, text_StHenry },
    /* 178 2026-07-14 */ { ORDINARY_TIME, 14, false, false, text_StCamillusDeLellis },
    /* 179 2026-07-15 */ { ORDINARY_TIME, 14, false, false, text_StBonaventure },
    /* 180 2026-07-16 */ { ORDINARY_TIME, 14, false, false, text_OurLadyOfMountCarmel },
    /* 181 2026-07-17 */ { ORDINARY_TIME, 14, false, true, NULL },
    /* 182 2026-07-18 */ { ORDINARY_TIME, 14, false, false, NULL },
    /* 183 2026-07-19 */ { ORDINARY_TIME, 15, true, false, NULL },
    /* 184 2026-07-20 */ { ORDINARY_TIME, 15, false, false, text_StApollinaris },
    /* 185 2026-07-21 */ { ORDINARY_TIME, 15, false, false, text_StLawrenceBrindisi },
    /* 186 2026-07-22 */ { ORDINARY_TIME, 15, false, false, text_StMaryMagdalene },
    /* 187 2026-07-23 */ { ORDINARY_TIME, 15, false, false, text_StBridget },
    /* 188 2026-07-24 */ { ORDINARY_TIME, 15, false, true, text_StSharbelMakhluf },
    /* 189 2026-07-25 */ { ORDINARY_TIME, 15, false, false, text_StJamesAp },
    /* 190 2026-07-26 */ { ORDINARY_TIME, 16, true, false, text_StsJoachimAnne },
    /* 191 2026-07-27 */ { ORDINARY_TIME, 16, false, false, NULL },
    /* 192 2026-07-28 */ { ORDINARY_TIME, 16, false, false, NULL },
    /* 193 2026-07-29 */ { ORDINARY_TIME, 16, false, false, text_StMartha },
    /* 194 2026-07-30 */ { ORDINARY_TIME, 16, false, false, text_StPeterChrysologus },
    /* 195 2026-07-31 */ { ORDINARY_TIME, 16, false, true, text_StIgnatiusLoyola },
    /* 196 2026-08-01 */ { ORDINARY_TIME, 16, false, false, text_StAlphonsusMariaDeLiguori },
    /* 197 2026-08-02 */ { ORDINARY_TIME, 17, true, false, text_StEusebius },
    /* 198 2026-08-03 */ { ORDINARY_TIME, 17, false, false, NULL },
    /* 199 2026-08-04 */ { ORDINARY_TIME, 17, false, false, text_StJeanVianney },
    /* 200 2026-08-05 */ { ORDINARY_TIME, 17, false, false, text_DedicationStMaryMajor },
    /* 201 2026-08-06 */ { ORDINARY_TIME, 17, false, false, text_Transfiguration },
    /* 202 2026-08-07 */ { ORDINARY_TIME, 17, false, true, text_StSixtusIIPope },
    /* 203 2026-08-08 */ { ORDINARY_TIME, 17, false, false, text_StDominic },
    /* 204 2026-08-09 */ { ORDINARY_TIME, 18, true, false, text_StEdithStein },
    /* 205 2026-08-10 */ { ORDINARY_TIME, 18, false, false, text_StLawrenceDeacon },
    /* 206 2026-08-11 */ { ORDINARY_TIME, 18, false, false, text_StClare },
    /* 207 2026-08-12 */ { ORDINARY_TIME, 18, false, false, text_StJaneFrancesDeChantal },
    /* 208 2026-08-13 */ { ORDINARY_TIME, 18, false, false, text_StsPontianHippolytus },
    /* 209 2026-08-14 */ { ORDINARY_TIME, 18, false, true, text_StMaximilianKolbe },
    /* 210 2026-08-15 */ { ORDINARY_TIME, 18, true, false, text_Assumption },
    /* 211 2026-08-16 */ { ORDINARY_TIME, 19, true, false, text_StStephenHungary },
    /* 212 2026-08-17 */ { ORDINARY_TIME, 19, false, false, NULL },
    /* 213 2026-08-18 */ { ORDINARY_TIME, 19, false, false, NULL },
    /* 214 2026-08-19 */ { ORDINARY_TIME, 19, false, false, text_StJohnEudes },
    /* 215 2026-08-20 */ { ORDINARY_TIME, 19, false, false, text_StBernardClairvaux },
    /* 216 2026-08-21 */ { ORDINARY_TIME, 19, false, true, text_StPiusX },
    /* 217 2026-08-22 */ { ORDINARY_TIME, 19, false, false, text_QueenshipMary },
    /* 218 2026-08-23 */ { ORDINARY_TIME, 20, true, false, text_StRoseLima },
    /* 219 2026-08-24 */ { ORDINARY_TIME, 20, false, false, text_StBartholomewAp },
    /* 220 2026-08-25 */ { ORDINARY_TIME, 20, false, false, text_StLouis },
    /* 221 2026-08-26 */ { ORDINARY_TIME, 20, false, false, NULL },
    /* 222 2026-08-27 */ { ORDINARY_TIME, 20, false, false, text_StMonica },
    /* 223 2026-08-28 */ { ORDINARY_TIME, 20, false, true, text_StAugustineHippo },
    /* 224 2026-08-29 */ { ORDINARY_TIME, 20, false, false, text_BeheadingJohnBaptist },
    /* 225 2026-08-30 */ { ORDINARY_TIME, 21, true, false, NULL },
    /* 226 2026-08-31 */ { ORDINARY_TIME, 21, false, false, NULL },
    /* 227 2026-09-01 */ { ORDINARY_TIME, 21, false, false, NULL },
    /* 228 2026-09-02 */ { ORDINARY_TIME, 21, false, false, NULL },
    /* 229 2026-09-03 */ { ORDINARY_TIME, 21, false, false, text_StGregoryGreat },
    /* 230 2026-09-04 */ { ORDINARY_TIME, 21, false, true, NULL },
    /* 231 2026-09-05 */ { ORDINARY_TIME, 21, false, false, text_StMotherTeresa },
    /* 232 2026-09-06 */ { ORDINARY_TIME, 22, true, false, NULL },
    /* 233 2026-09-07 */ { ORDINARY_TIME, 22, false, false, NULL },
    /* 234 2026-09-08 */ { ORDINARY_TIME, 22, false, false, text_NativityVirginMary },
    /* 235 2026-09-09 */ { ORDINARY_TIME, 22, false, false, text_StPeterClaver },
    /* 236 2026-09-10 */ { ORDINARY_TIME, 22, false, false, NULL },
    /* 237 2026-09-11 */ { ORDINARY_TIME, 22, false, true, NULL },
    /* 238 2026-09-12 */ { ORDINARY_TIME, 22, false, false, text_HolyNameMary },
    /* 239 2026-09-13 */ { ORDINARY_TIME, 23, true, false, text_StJohnChrysostom },
    /* 240 2026-09-14 */ { ORDINARY_TIME, 23, false, false, text_ExaltationCross },
    /* 241 2026-09-15 */ { ORDINARY_TIME, 23, false, false, text_OurLadyOfSorrows },
    /* 242 2026-09-16 */ { ORDINARY_TIME, 23, false, true, text_StsCorneliusCyprian },
    /* 243 2026-09-17 */ { ORDINARY_TIME, 23, false, false, text_StRobertBellarmine },
    /* 244 2026-09-18 */ { ORDINARY_TIME, 23, false, true, NULL },
    /* 245 2026-09-19 */ { ORDINARY_TIME, 23, false, true, text_StJanuarius },
    /* 246 2026-09-20 */ { ORDINARY_TIME, 24, true, false, text_StAndrewKimTaegon },
    /* 247 2026-09-21 */ { ORDINARY_TIME, 24, false, false, text_StMatthewEvangelist },
    /* 248 2026-09-22 */ { ORDINARY_TIME, 24, false, false, NULL },
    /* 249 2026-09-23 */ { ORDINARY_TIME, 24, false, false, text_StPioPietrelcina },
    /* 250 2026-09-24 */ { ORDINARY_TIME, 24, false, false, NULL },
    /* 251 2026-09-25 */ { ORDINARY_TIME, 24, false, true, NULL },
    /* 252 2026-09-26 */ { ORDINARY_TIME, 24, false, false, text_StsCosmasDamian },
    /* 253 2026-09-27 */ { ORDINARY_TIME, 25, true, false, text_StVincentDePaul },
    /* 254 2026-09-28 */ { ORDINARY_TIME, 25, false, false, text_StWenceslaus },
    /* 255 2026-09-29 */ { ORDINARY_TIME, 25, false, false, text_StsArchangels },
    /* 256 2026-09-30 */ { ORDINARY_TIME, 25, false, false, text_StJerome },
    /* 257 2026-10-01 */ { ORDINARY_TIME, 25, false, false, text_StThereseChildJesus },
    /* 258 2026-10-02 */ { ORDINARY_TIME, 25, false, true, text_GuardianAngels },
    /* 259 2026-10-03 */ { ORDINARY_TIME, 25, false, false, NULL },
    /* 260 2026-10-04 */ { ORDINARY_TIME, 26, true, false, text_StFrancisAssisi },
    /* 261 2026-10-05 */ { ORDINARY_TIME, 26, false, false, text_StFaustinaKowalska },
    /* 262 2026-10-06 */ { ORDINARY_TIME, 26, false, false, text_StBruno },
    /* 263 2026-10-07 */ { ORDINARY_TIME, 26, false, false, text_OurLadyOfTheRosary },
    /* 264 2026-10-08 */ { ORDINARY_TIME, 26, false, false, NULL },
    /* 265 2026-10-09 */ { ORDINARY_TIME, 26, false, true, text_StDenis },
    /* 266 2026-10-10 */ { ORDINARY_TIME, 26, false, false, NULL },
    /* 267 2026-10-11 */ { ORDINARY_TIME, 27, true, false, NULL },
    /* 268 2026-10-12 */ { ORDINARY_TIME, 27, false, false, NULL },
    /* 269 2026-10-13 */ { ORDINARY_TIME, 27, false, false, NULL },
    /* 270 2026-10-14 */ { ORDINARY_TIME, 27, false, false, text_StCallistusIPope },
    /* 271 2026-10-15 */ { ORDINARY_TIME, 27, false, false, text_StTeresaJesus },
    /* 272 2026-10-16 */ { ORDINARY_TIME, 27, false, true, text_StHedwig },
    /* 273 2026-10-17 */ { ORDINARY_TIME, 27, false, false, text_StIgnatiusAntioch },
    /* 274 2026-10-18 */ { ORDINARY_TIME, 28, true, false, text_StLukeEvangelist },
    /* 275 2026-10-19 */ { ORDINARY_TIME, 28, false, false, text_StsJeanBrebeuf },
    /* 276 2026-10-20 */ { ORDINARY_TIME, 28, false, false, NULL },
    /* 277 2026-10-21 */ { ORDINARY_TIME, 28, false, false, NULL },
    /* 278 2026-10-22 */ { ORDINARY_TIME, 28, false, false, text_StJohnPaulII },
    /* 279 2026-10-23 */ { ORDINARY_TIME, 28, false, true, text_StJohnCapistrano },
    /* 280 2026-10-24 */ { ORDINARY_TIME, 28, false, false, text_StAnthonyMaryClaret },
    /* 281 2026-10-25 */ { ORDINARY_TIME, 29, true, false, NULL },
    /* 282 2026-10-26 */ { ORDINARY_TIME, 29, false, false, NULL },
    /* 283 2026-10-27 */ { ORDINARY_TIME, 29, false, false, NULL },
    /* 284 2026-10-28 */ { ORDINARY_TIME, 29, false, false, text_StSimonStJudeAp },
    /* 285 2026-10-29 */ { ORDINARY_TIME, 29, false, false, NULL },
    /* 286 2026-10-30 */ { ORDINARY_TIME, 29, false, true, NULL },
    /* 287 2026-10-31 */ { ORDINARY_TIME, 29, false, false, NULL },
    /* 288 2026-11-01 */ { ORDINARY_TIME, 30, true, false, text_AllSaints },
    /* 289 2026-11-02 */ { ORDINARY_TIME, 30, false, false, text_AllSouls },
    /* 290 2026-11-03 */ { ORDINARY_TIME, 30, false, false, text_StMartinPorres },
    /* 291 2026-11-04 */ { ORDINARY_TIME, 30, false, false, text_StCharlesBorromeo },
    /* 292 2026-11-05 */ { ORDINARY_TIME, 30, false, false, NULL },
    /* 293 2026-11-06 */ { ORDINARY_TIME, 30, false, true, NULL },
    /* 294 2026-11-07 */ { ORDINARY_TIME, 30, false, false, NULL },
    /* 295 2026-11-08 */ { ORDINARY_TIME, 31, true, false, NULL },
    /* 296 2026-11-09 */ { ORDINARY_TIME, 31, false, false, text_DedicationLateran },
    /* 297 2026-11-10 */ { ORDINARY_TIME, 31, false, false, text_StLeoGreat },
    /* 298 2026-11-11 */ { ORDINARY_TIME, 31, false, false, text_StMartinTours },
    /* 299 2026-11-12 */ { ORDINARY_TIME, 31, false, false, text_StJosaphat },
    /* 300 2026-11-13 */ { ORDINARY_TIME, 31, false, true, NULL },
    /* 301 2026-11-14 */ { ORDINARY_TIME, 31, false, false, NULL },
    /* 302 2026-11-15 */ { ORDINARY_TIME, 32, true, false, text_StAlbertGreat },
    /* 303 2026-11-16 */ { ORDINARY_TIME, 32, false, false, text_StMargaretScotland },
    /* 304 2026-11-17 */ { ORDINARY_TIME, 32, false, false, text_StElizabethHungary },
    /* 305 2026-11-18 */ { ORDINARY_TIME, 32, false, false, text_DedicationStsPeterPaul },
    /* 306 2026-11-19 */ { ORDINARY_TIME, 32, false, false, NULL },
    /* 307 2026-11-20 */ { ORDINARY_TIME, 32, false, true, NULL },
    /* 308 2026-11-21 */ { ORDINARY_TIME, 32, false, false, text_PresentationMary },
    /* 309 2026-11-22 */ { ORDINARY_TIME, 33, true, false, text_ChristKing },
    /* 310 2026-11-23 */ { ORDINARY_TIME, 33, false, false, text_StClementIPope },
    /* 311 2026-11-24 */ { ORDINARY_TIME, 33, false, false, text_StAndrewDungLac },
    /* 312 2026-11-25 */ { ORDINARY_TIME, 33, false, false, text_StCatherineAlexandria },
    /* 313 2026-11-26 */ { ORDINARY_TIME, 33, false, false, NULL },
    /* 314 2026-11-27 */ { ORDINARY_TIME, 33, false, true, NULL },
    /* 315 2026-11-28 */ { ORDINARY_TIME, 33, false, false, NULL },
    /* 316 2026-11-29 */ { ADVENT, 1, true, false, NULL },
    /* 317 2026-11-30 */ { ADVENT, 1, false, false, text_StAndrewAp },
    /* 318 2026-12-01 */ { ADVENT, 1, false, false, NULL },
    /* 319 2026-12-02 */ { ADVENT, 1, false, false, NULL },
    /* 320 2026-12-03 */ { ADVENT, 1, false, false, text_StFrancisXavier },
    /* 321 2026-12-04 */ { ADVENT, 1, false, true, text_StJohnDamascene },
    /* 322 2026-12-05 */ { ADVENT, 1, false, false, NULL },
    /* 323 2026-12-06 */ { ADVENT, 2, true, false, text_StNicholas },
    /* 324 2026-12-07 */ { ADVENT, 2, false, false, text_StAmbrose },
    /* 325 2026-12-08 */ { ADVENT, 2, true, false, text_ImmaculateConception },
    /* 326 2026-12-09 */ { ADVENT, 2, false, false, text_JuanDiego },
    /* 327 2026-12-10 */ { ADVENT, 2, false, false, text_OurLadyOfLoreto },
    /* 328 2026-12-11 */ { ADVENT, 2, false, true, text_StDamasusIPope },
    /* 329 2026-12-12 */ { ADVENT, 2, false, false, text_OurLadyOfGuadalupe },
    /* 330 2026-12-13 */ { ADVENT, 3, true, false, text_StLucySyracuse },
    /* 331 2026-12-14 */ { ADVENT, 3, false, false, text_StJohnCross },
    /* 332 2026-12-15 */ { ADVENT, 3, false, false, NULL },
    /* 333 2026-12-16 */ { ADVENT, 3, false, true, NULL },
    /* 334 2026-12-17 */ { ADVENT, 3, false, false, NULL },
    /* 335 2026-12-18 */ { ADVENT, 3, false, true, NULL },
    /* 336 2026-12-19 */ { ADVENT, 3, false, true, NULL },
    /* 337 2026-12-20 */ { ADVENT, 4, true, false, NULL },
    /* 338 2026-12-21 */ { ADVENT, 4, false, false, text_StPeterCanisius },
    /* 339 2026-12-22 */ { ADVENT, 4, false, false, NULL },
    /* 340 2026-12-23 */ { ADVENT, 4, false, false, text_StJohnKanty },
    /* 341 2026-12-24 */ { ADVENT, 4, false, false, NULL },
    /* 342 2026-12-25 */ { CHRISTMAS, 1, true, true, text_Christmas },
    /* 343 2026-12-26 */ { CHRISTMAS, 1, false, false, text_StStephenProtomartyr },
    /* 344 2026-12-27 */ { CHRISTMAS, 2, true, false, text_HolyFamily },
    /* 345 2026-12-28 */ { CHRISTMAS, 2, false, false, text_HolyInnocents },
    /* 346 2026-12-29 */ { CHRISTMAS, 2, false, false, text_StThomasBecket },
    /* 347 2026-12-30 */ { CHRISTMAS, 2, false, false, NULL },
    /* 348 2026-12-31 */ { CHRISTMAS, 2, false, false, text_StSylvesterIPope },
};
