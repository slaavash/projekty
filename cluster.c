/*
    IZP - Projekt 2 - Prace s datovymi strukturami
    Autor: Michal Slavotinek (xslavo03)
    s/o autorovi kostry: https://moodle.vut.cz/pluginfile.php/514187/mod_page/content/6/cluster.c
    special thanks to sigsegv, sigabrt & valgrind <3
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <limits.h>

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

struct obj_t { //struktura objektu
    int id; //idetifikator
    float x; //souradnice X
    float y; //souradnice Y
};

struct cluster_t { //shluk objektu
    int size; //pocet objektu ve shluku
    int capacity; //pocet objektu, pro ktere je alokovano misto
    struct obj_t *obj; //ukazatel na pole shluku
};

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap) //TODO
{
    assert(c != NULL);
    assert(cap >= 0);

    c->size = 0; //pocet objektu ve shluku = 0
    if(cap > 0) //pokud je zadouci kapacita > 0
    {
        c->obj = malloc(cap*sizeof(struct obj_t)); //alokace pameti pro cap objektu
        if(c->obj != NULL) //pokud se povedlo alokovat
        {
        c->capacity = cap;
        return;
        }
        else //pokud se alokace nezdarila
        {
        c->capacity = 0;
        return;
        }
    }
    else //pokud je spatne zadana kapacita
    {
        c->capacity = 0;
        c->obj = NULL;
        return;
    }
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c) //TODO
{
    free(c->obj); //uvolneni pamet alokovanou pro shluk
    init_cluster(c, 0); //incialituje shluk na kapacitu 0
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj) //TODO
{
   if(c->capacity == c->size) //pokud neni dost mista
   {
         resize_cluster(c, c->capacity+ CLUSTER_CHUNK); //zavolej resize_cluster
   }
    c->obj[c->size] = obj; //pridej objekt na posledni misto shluku
    c->size++; //inkrementuj pocet objektu
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2) //TODO
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    int c1size = c1->size; //uklada puvodni pocet objektu v C1
    for(int i = 0; i < c2->size; i++) //pro vsechny objekty v c2
    {
        append_cluster(c1, c2->obj[i]); //uklada objekty z c2 do c1
    }
    if(c1->size == c1size + c2->size) //pokud se povedlo
    {
        sort_cluster(c1); //serad shluk c1
    }
    
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx) //TODO
{
    assert(idx < narr);
    assert(narr > 0);

    int novynarr = narr -1; //zmensi pocet shluku v poli o jeden
    clear_cluster(&carr[idx]); //odstrani shluk na indexu idx 
    for(int i = idx; i < novynarr; i++) //pro vsechny shluky za indexem idx
    {
        carr[i] = carr[i+1]; //shluky se ulozi na indexu o jeden mensi
    }
    return novynarr; //vraci novy pocet shluku v poli
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2) //TODO
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    int vzdalenost, x, y;
    x = (o1->x) - (o2->x); //rozdil na souradnicich X
    y = (o1->y) - (o2->y); //rozdil na souradnicich Y
    vzdalenost = sqrtf((x*x) + (y*y)); //odmocnina z mocnin rozdilu souradnic
    return vzdalenost; //vraci vzdalenost
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2) //TODO
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

   float vzdalenost = 0.0; //vzdalenost dvou prvku
   float tmp; //pomocna promenna tmp
   for(int i = 0; i < c1->size; i++) //pro kazdy prvek z c1
   {
        for (int j = 0; j < c2->size; j++) //pro kazdy prvek z c2
        {
            tmp = obj_distance(&c1->obj[i], &c2->obj[j]); //pocita vzdalenost dvou objektu z dvou shluku
            if(tmp > vzdalenost) //pokud je vzdalenost mensi nez tmp
            {
                vzdalenost = tmp; //nova vzdalenost
            }
        }
        
   }
   return vzdalenost;
}


/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2) //TODO
{
    assert(narr > 0);

    if (narr == 1) //pokud pole obsahuje pouze jednu polozku
    {
       *c1 = *c2 = 0; //vrat nulu na obou indexech
        return;
    }
    float vzdalenost = 0, tmp; //promenna vzdalenost a pomocna
    for (int i = 0; i < narr-1; i++) //pro vsechny prvky pole - posledni
    {
        for (int j = i + 1; j < narr; j++) //pro prvky pole vetsi alespon o 1
        {
            tmp = cluster_distance(&carr[i], &carr[j]); //vypocitej vzdalenost do tmp
            if (vzdalenost == 0 || tmp < vzdalenost) //pokud se vzdalenost = 0(jeste neni nenulova) nebo pokud tmp < vzdalenost
            {
                vzdalenost = tmp; //nova vzdalenost je tmp
                *c1 = i; //novy index c1 = i
                *c2 = j; //novy index c2 = j
            }
        }
   }
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);
    int id, psh, pn = 0; //pomocna promenna pro id, ocekavany PocetSHluku, PocetNactenych
    float x,y; //pomocne promenne pro souradnice
    *arr = NULL;
    struct obj_t objekt; 
    
    FILE *soubor = fopen(filename, "r"); //otevri soubor
    if(soubor == NULL) //pokud otevirani selhalo
    {
        fprintf(stderr, "Otevreni souboru selhalo\n"); //vypis chybu
        fclose(soubor); //zavri soubor
        return 0; //konec
    }
    fscanf(soubor, "count=%d\n", &psh); //nacti pocet objektu
    if(psh > INT_MAX || psh < 1) //pokud nesplnuji limit
    {
        fprintf(stderr, "Pocet objektu je neplatny\n"); //vypis chybu
        fclose(soubor); //zavri soubor
        return 0; //konec
    }
    *arr = malloc(psh * sizeof(struct cluster_t)); //alokuj pole pro velikost psh shluku
    if((*arr) == NULL) //pokud alokace selhala
    {
        fprintf(stderr, "Alokace clusteru selhalo\n"); //vypis chybu
        fclose(soubor); //zavri soubor
        free(*arr); //uvolni pole
        return 0; //konec
    }
    for(int i = 0; i < psh; i++) //pro ocekavany pocet objektu
    {
        init_cluster(&(*arr)[i], CLUSTER_CHUNK); //inicialituj pamet
        fscanf(soubor, "%d %f %f\n", &id, &x, &y); //precti ze souboru
        if(x < 0 || x > 1000 || y < 0 || y > 1000) //pokud souradnice nesplnuji podminky
        {
            fprintf(stderr, "Souradnice nesplnuji podminku\n"); //vypis chybu
            free(*arr); //uvolni pole
            fclose(soubor); //zavri soubor
            return 0; //konec
        }
        objekt.id = id; //prirazeni id
        objekt.x = x; //prirazeni x
        objekt.y = y; //prirazenhi y
        append_cluster(&(*arr)[i], objekt); //nacteni do pole
        pn = pn + 1; //zvetseni poctu nactench
        if(psh == pn) //pokud uspesne nacetl ocekavany pocet shluku
        {
            break; //prerus
        }
    }
    fclose(soubor); //zavri soubor
    return pn; //vrat pocet nactenych
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[]) //TODO
{
    if(argc == 1 || argc > 3) //pokud je program spusten bez nazvu souboru nebo s vice argumenty
    {
        fprintf(stderr,"Spatne zadany argument\n"); //vypis chybu
        return 1; //konec
    }
    int pocet_shluku, pocet_nactenych; //pozadovany pocet shluku
    if(argc == 3) //pokud je zadan pocet shluku
    {
        pocet_shluku = (int) strtol(argv[2], NULL, 10); //nacti
        if(pocet_shluku <= 0 || pocet_shluku > INT_MAX){ //poku to neni int
            fprintf(stderr, "Spatne zadany argument\n"); //vypis chybu
            return 1; //konec
        }
            
    }
    else //pokud neni zadany pocet shluku
    {
        pocet_shluku = 1; //pozadovany pocet shluku = 1
    }
    struct cluster_t *pole;
    pocet_nactenych = load_clusters(argv[1],  &pole); //vraci pocet nactenych
    if(pocet_nactenych == 0) //pokud nenacetl
    {
        return 1; //konec
    }
    else if(pocet_nactenych < pocet_shluku) //pokud je vice pozadovanych nez nactenych
	{    
        fprintf(stderr, "Pocet nactenych < pocet zadoucich\n"); //vypis chybu
	    int tmp = pocet_nactenych; //pomocna promenna, abych nemenil pocet nactenych
        while(1){ 
        if(tmp!=0){ //dokud tmp nedojde na nulu
            clear_cluster(&pole[tmp-1]);
            tmp--; //dekrementuj
        }
        else{ //pokud tmp = 0
            break; //prerus
        }
        }
        free(pole); //uvolni pole
        return 1; //konec
    }
    else if(pocet_nactenych == pocet_shluku) //pokud pocet pozadovanych shluku = pocet nactenych
    {
        print_clusters(pole, pocet_shluku); //vytiskni vsechny
        int tmp = pocet_nactenych; //pomocna promenna, abych nemenil pocet nactenych
        while(1){ 
        if(tmp!=0){ //dokud tmp nedojde na nulu
            clear_cluster(&pole[tmp-1]);
            tmp--; //dekrementuj
        }
        else{ //pokud tmp = 0
            break; //prerus
        }
        }
        free(pole); //uvolni pole
        return 0; //konec
        
    }
    int pocet_mergu = pocet_nactenych - pocet_shluku; //pocet, kolik mergu se provede k uspokojeni pozadovanych shluku 
    int a,b; //pomocna promenna  a, b, k ukladani indexu
    int tmp = pocet_nactenych; //pomocna promenna, abych nemenil pocet nactenych
    for(int i = 0; i<pocet_mergu; i++) //merguj pozadovany pocet
    {
        a=0; b=0; //nuluj indexy
        find_neighbours(pole, tmp, &a, &b); //najdi nejblizsi shluky
        merge_clusters(&pole[a], &pole[b]); //spoj nalezene shluky
        clear_cluster(&pole[b]); //vymaz shluk na indexu b
        remove_cluster(pole, tmp, b); //odstran shluk
        tmp--; //dekrementace poctu zbyvajicich 
    }
    print_clusters(pole, pocet_shluku); //vytiskni shluky
    while(1){ 
        if(tmp!=0){ //dokud pocet zbyvajicich neni 0
            clear_cluster(&pole[tmp-1]); //vymaz shluk na indexu tmp-1
            tmp--; //dekrementace poctu zbyvajicich
        }
        else{ //pokud tmp = 0
            break; //prerus
        }
    }
    free(pole); //uvolni pole
    return 0; //konec
}
