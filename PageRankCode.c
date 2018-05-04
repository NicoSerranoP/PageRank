#include<stdio.h>
#include<stdlib.h>
#include<math.h>

// Print matrix function
void
impMat (float *matrix, const int n)
{
  for (int i = 0; i < n; i++)
    {
      for (int j = 0; j < n; j++)
	printf ("%f ", *matrix++);
      puts ("\n");
    }
  // Use this command in order to print matrix
  //impMat(&a[0][0],n);
}


//..............................................................
int
main (void)
{
  // file pointer
  FILE *fp;
  // n is the size of the matrix
  int n=0;
  int ch,nodef,nodet;

  fp=fopen("links.txt","r");
  while ((ch = fgetc(fp)) != EOF)
  {

      fscanf(fp,"%d%d", &nodef, &nodet);
  	  if(nodef >= n)
        n=nodef + 1;
      else if (nodet >= n)
        n=nodet + 1;
  }
  printf("La matriz será de %d X %d \n\n", n,n);
  fclose(fp);

  // Matrix
  float a[n][n];
  // Initializing to zero all elements
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      a[i][j] = 0.0;

  //Matrix nodes
  int nodefrom, nodeto;
  // open file again
  fp = fopen ("links.txt", "r");

  // get the connections between nodes
  puts ("Nodes checking");
  while (!feof (fp))
    {
      fscanf (fp, "%d%d", &nodefrom, &nodeto);
      a[nodefrom][nodeto] = 1.0;
      printf ("%d %d \n", nodefrom, nodeto);
    }
  // close the file
  fclose (fp);


  // Use this command in order to print matrix
  puts ("Matrix A:");
  impMat (&a[0][0], n);

  // counting the outlinks from each page
  int out_link[n];
  // initialize vector to zero
  for (int i = 0; i < n; i++)
    out_link[i] = 0;

  //out_link vector indicates how many links go out from page [row]
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
	{
	  if (a[i][j] != 0.0)
	  {
	      out_link[i] = out_link[i] + 1;
	  }
	}
  }

  //printing the out_link vector
  puts ("out_link vector:");
  for (int i = 0; i < n; i++)
    printf (" %d \n", out_link[i]);

  // creating the stochastic matrix A dividing each A[i][j] by out_link[i]
  for (int i = 0; i < n; i++)
  {
    /* if it is a PDF or a no out_link page, the possibility of going there is
    the probability to going to every page (you can't divide by zero) */
    if (out_link[i] == 0)
	{
	  for (int j = 0; j < n; j++)
	    a[i][j] = 1.0 / n;
	}
      // A[i][j] divided by out_link[i]
    else
	{
	  for (int j = 0; j < n; j++)
	    a[i][j] = a[i][j] / out_link[i];
	}
  }

  puts ("Stochastic Matrix A");
  impMat (&a[0][0], n);

  // creating the A Transposed matrix (At)
  float at[n][n];
  for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
	    {
	        at[i][j] = a[j][i];
	    }
    }

  //...................................................................

  // d is the damping factor
  float d = 0.85;
  // Pagerank loop switch
  int looping = 1;
  // Set the looping condition and the number of iterations 'k'
  int k = 0;

  //x[n] is the random surfer vector (initial)
  float x[n];
  for (int i = 0; i < n; i++)
    {
        x[i] = 1.0 / n;
    }

  // Initialize new b[] vector
  float b[n];

  // Imagine the system like Ax = b
  while (looping)
    {

    for (int i = 0; i < n; i++)
	{
	  b[i] = 0.0;
	}
    // Update b[] (without using the damping factor)
    for (int i = 0; i < n; i++)
	{
	  for (int j = 0; j < n; j++)
	    {
	      b[i] = b[i] + (at[i][j] * x[j]);
	    }
	}

    // Update b[] (using the damping factor)
    for (int i = 0; i < n; i++)
	{
	  b[i] = d * b[i] + (1.0 - d) / n;
	}

    float error = 0.0;
    for (int i = 0; i < n; i++)
	{
	  error = error + fabs (b[i] - x[i]);
	}

    //if two consecutive instances of pagerank vector are almost identical, stop
    if (error < 0.000001)
	{
	  looping = 0;
	}

    // Update x[] with our new b[]
    for (int i = 0; i < n; i++)
	{
	  x[i] = b[i];
	}

    // Increase the number of iterations
    k = k + 1;
    }

  puts ("Egeinvector: ");

  for (int i = 0; i < n; i++)
    printf ("%f \n", x[i]);

  printf("Iterations: %d \n",k);


}
