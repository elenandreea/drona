#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INFINITY 9999999


int dijk(int **matrice,int n, int nod_initial,int nod_destinatie,int ultimul,FILE *iesire,int ID,int ok);

//alocarea unei matrici
int** initGraph(int n)
{
	int **mat;
	mat = (int**)calloc(n,sizeof(int*));
	int i;
	for(i=0;i<n;i++)
		mat[i] = (int*)calloc(n,sizeof(int));
	return mat;
}

//adaugarea unei muchii intre noduri
void addEdge(int u, int v, int**mat,int cost)
{
	mat[u][v]=cost;
}

//functie de afisare a matricii
void show(int** mat,int n)
{
	int i ,j;
	for(i =0; i<n ;i++)
	{
		for (j =0 ; j<n;j++)
			printf("%d ",mat[i][j] );
		printf("\n");
	}
}

//TASK1
void livrare_ferma(FILE *intrare,FILE *iesire,int ID)
{
	int nr_noduri;
	fscanf(intrare,"%d",&nr_noduri);
	//vector pentru tipul fermei
	int *vect_noduri;
	vect_noduri = (int*)calloc(nr_noduri,sizeof(int));
	int i;
	int ferma;
	for(i=0;i<nr_noduri;i++)
	{
		fscanf(intrare,"%d",&vect_noduri[i]);
		if(vect_noduri[i]==1)
			ferma = i;	
	}

	int nr_drumuri;
	fscanf(intrare,"%d",&nr_drumuri);

	//crearea matricei de costuri
	int **matrice=NULL;
	matrice=initGraph(nr_noduri);

	int nod1,nod2,cost;
	for(i=0;i<nr_drumuri;i++)
	{
		fscanf(intrare,"%d%d%d",&nod1,&nod2,&cost);
		addEdge(nod1,nod2,matrice,cost);
	}

	int ok=0;
	int ultimul=0;
	int cale=0;
	int total=0;
	//se memoreaza comenzile intr un vector
	int nr_comenzi,tip;
	fscanf(intrare,"%d",&nr_comenzi);
	int *vect_comenzi;
	vect_comenzi = (int*)calloc(nr_comenzi,sizeof(int));
	for(i=0;i<nr_comenzi;i++)
	{
		fscanf(intrare,"%d %d",&tip,&vect_comenzi[i]);
		//calculul distantei de la ferma la destinatie
		cale=dijk(matrice,nr_noduri,ferma,vect_comenzi[i],ultimul,iesire,ID,ok);
		total =total+cale;

		if(i==nr_comenzi-1)
			ultimul=1;

		//calculul distantei de la destinatie la ferma
		cale=dijk(matrice,nr_noduri,vect_comenzi[i],ferma,ultimul,iesire,ID,ok);
		total=total+cale;
	}

	fprintf(iesire,"%d\n",total);

	//dealocare
	free(vect_noduri);
	for(i=0;i<nr_noduri;i++)
		free(matrice[i]);
	free(matrice);
	free(vect_comenzi);
}

void livrare_ferme(FILE *intrare,FILE *iesire,int ID)
{
	int nr_noduri;
	fscanf(intrare,"%d",&nr_noduri);
	//vector pentru tipul fermei
	int *vect_noduri;
	vect_noduri = (int*)calloc(nr_noduri,sizeof(int));
	int i;
	
	for(i=0;i<nr_noduri;i++)
	{
		fscanf(intrare,"%d",&vect_noduri[i]);	
	}

	int nr_drumuri;
	fscanf(intrare,"%d",&nr_drumuri);

	//crearea matricei de costuri
	int **matrice=NULL;
	matrice=initGraph(nr_noduri);

	int nod1,nod2,cost;
	for(i=0;i<nr_drumuri;i++)
	{
		fscanf(intrare,"%d%d%d",&nod1,&nod2,&cost);
		addEdge(nod1,nod2,matrice,cost);
	}

	int ultimul=0;
	int cale=0;
	int total=0;
	//se memoreaza comenzile intr un vector
	int nr_comenzi,tip;
	fscanf(intrare,"%d",&nr_comenzi);
	int *vect_comenzi;
	vect_comenzi = (int*)calloc(nr_comenzi,sizeof(int));

	int ferma;
	for(i=0;i<nr_comenzi;i++)
		fscanf(intrare,"%d %d",&tip,&vect_comenzi[i]);

	fscanf(intrare,"%d",&ferma);

	int ok;
	for(i=0;i<nr_comenzi;i++)
	{	
		//se determina drumul dus
		ok=0;
		cale=dijk(matrice,nr_noduri,ferma,vect_comenzi[i],ultimul,iesire,ID,ok);
		total =total+cale;

		if(i==nr_comenzi-1)
			ultimul=1;
		
		//se determina drumul intors
		ok=1;
		int m;
		int cale_minima=INFINITY;
		for(m=0;m<nr_noduri;m++)

			if(vect_noduri[m]==1)
			{
				if(i==nr_comenzi-1)
				{	
					cale=dijk(matrice,nr_noduri,vect_comenzi[i],m,ultimul,iesire,ID,ok);
					if(cale < cale_minima)
					{
						cale_minima=cale;
						ferma=m;
					}
				}
				else
				{
					//se determina cel mai eficient drum.se va calcula calea totala
					cale=dijk(matrice,nr_noduri,vect_comenzi[i],m,ultimul,iesire,ID,ok);
					cale=cale+dijk(matrice,nr_noduri,m,vect_comenzi[i+1],ultimul,iesire,ID,ok);
					if(cale<cale_minima)
					{
						cale_minima=cale;
						ferma=m;
					}

				}
			}
		if(m==nr_noduri)
			ok=0;

		cale_minima=dijk(matrice,nr_noduri,vect_comenzi[i],ferma,ultimul,iesire,ID,ok);
		total=total+cale_minima;

	}

	fprintf(iesire,"%d\n",total);

	free(vect_noduri);
	for(i=0;i<nr_noduri;i++)
		free(matrice[i]);
	free(matrice);
	free(vect_comenzi);
}

int dijk(int **matrice,int n, int nod_initial,int nod_destinatie,int ultimul,FILE *iesire,int ID,int ok)
{
	int **mat_cost=NULL;
	mat_cost=initGraph(n);
	int *vect_dist = (int*)calloc(n,sizeof(int));
	int *parinte = (int*)calloc(n,sizeof(int));
	int *vizitat = (int*)calloc(n,sizeof(int));
	int index=1;
	int dist_min;
	int urm_nod;
	int i,j;

	//pentru valorile nule se initializeaza pozitiile cu o valoare foarte mare
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			if(matrice[i][j]==0)
				mat_cost[i][j]=INFINITY;
			else
				mat_cost[i][j]=matrice[i][j];

	//se initializeza vectorul de distante(costuri)
	//vectorul de parinti primeste valoarea nodului initial
	for(i=0;i<n;i++)
	{
		vect_dist[i]=mat_cost[nod_initial][i];
		parinte[i]=nod_initial;
		vizitat[i]=0;
	}

	//distanta de la nod la el insusi e mereu 0
	vect_dist[nod_initial]=0;
	vizitat[nod_initial]=1;
	//se marcheaza primul nod ca vizitat

	while(index<n-1)
	{
		//initial primeste o valoare mare
		dist_min=INFINITY;

		for(i=0;i<n;i++)
			//se determina distanta minima cat timp nodul nu e vizitat.
			//se retine nodul
			if(vect_dist[i]<dist_min && !vizitat[i])
			{
				dist_min=vect_dist[i];
				urm_nod=i;
			}

		//se viziteaza
		vizitat[urm_nod]=1;

		for(i=0;i<n;i++)
			if(vizitat[i]==0)
				if(dist_min+mat_cost[urm_nod][i]<vect_dist[i])
				{
					vect_dist[i]=dist_min+mat_cost[urm_nod][i];
					//se construieste un vector de parinti
					parinte[i]=urm_nod;
				}

		index=index+1;
	}

	int nod=nod_destinatie;
	int k =0;
	int drum[50]={0};

	//se retine calea inversa intr un vector
	while(nod!=nod_initial)
	{
		drum[k]=nod;
		nod=parinte[nod];
		k++;
	}
	
	drum[k]=nod_initial;
	
    if(ID==1)
    {
	    while(k>0)
	    {
	    	fprintf(iesire,"%d ", drum[k]);
	    	k--;
	    }

	    //ultimul e printat separat pentru a nu urma dupa el spatiu
	    if(ultimul==1)
	    	fprintf(iesire,"%d\n", drum[k]);
	}

	if(ID==2)
		if(ok==0)
		{
			while(k>0)
	    	{
	    		fprintf(iesire,"%d ", drum[k]);
	    		k--;
	    	}

	    	if(ultimul==1)
	    		fprintf(iesire,"%d\n", drum[k]);
		}
	int cost_final;
	cost_final = vect_dist[nod_destinatie];
	free(vect_dist);
    free(parinte);
    free(vizitat);
	for(i=0;i<n;i++)
		free(mat_cost[i]);
	free(mat_cost);
    return cost_final;  
}

int main(int argc, char **argv)
{
	if(argc==1)
	{
		printf("[Eroare]: Nu s-au dat argumente de comanda.\n");
		return 1;
	}

	if(argc==2)
	{
		printf("Nu avem fisier de output\n");
		return 1;
	}

	FILE *intrare;
	FILE *iesire;

	intrare = fopen(argv[1],"r");
	iesire = fopen(argv[2],"w");

	if(intrare==NULL)
	{
		printf("[Eroare]: Fisierul %s nu poate fi deschis.\n",argv[1]);
		return 0;
	}

	int ID;
	fscanf(intrare,"%d",&ID);

	if(ID==1)
		livrare_ferma(intrare,iesire,ID);
	if(ID==2)
		livrare_ferme(intrare,iesire,ID);


	fclose(intrare);
	fclose(iesire);
	return 0;
}