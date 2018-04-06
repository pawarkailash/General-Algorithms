#include <iostream>
#include <cstdlib>
#include <vector>

//!
//! Show the contents of vector
//! Vector is passed as constant object (as refrence)
//! Note that const_iteartor is used for visitng its elemnts
//!
void showList(const std::vector<int> &list)
{
  std::cout <<"Coefficients of polynomial: ";
  for (int i=0; i<list.size(); i++) {
    std::cout <<" " <<list[i]; 
  } 
  std::cout <<"\n";
}

//!
//! This function generates a list containing specified set of numbers
//! randomly bounded by limit specified by user
//!
void generateList(std::vector<int> &list, unsigned int count, 
        unsigned int limit)
{
  for (int i=0; i<count; i++) {
    list.push_back(rand() % limit);
  }
}

//!
//! Navie implementaion of polynomial multiplication, which is taught in
//! schools.
//!
int multiplyPolyNavie(const std::vector<int> &a, 
        const std::vector<int> &b, std::vector<int> &prod)
{
  // Initialize the product array: 
  prod.assign((2*a.size()-1), 0);
  
  unsigned count = 0;
  for (int i=0; i<a.size(); i++) {
    for (int j=0; j<a.size(); j++) {
      prod[i+j] += a[i] * b[j];
      count++;
    }
  }
  return count;
}

//!
//! Improved implementaion of polynomial multiplication, which has run time as
//! bad as quadratic
//!
int multiply2(int product[], int A[], int B[], 
        unsigned size, unsigned aIndex, unsigned bIndex, bool isLow, bool &isProd)
{

  bool isDebug = false;
  int result;
  if(size == 1) {
    result = A[aIndex] * B[bIndex];
    if(isDebug) std::cout <<"LOW[" <<isLow <<"]  [" <<aIndex <<"," <<bIndex <<"]  " <<result <<"\n";
    isProd = true;
    return result;    
  }

  int lowerProd = multiply2(product, A, B, size/2, aIndex, bIndex, true, isProd);
  for(int i=0; i<=(size-2); i++) {
    if(isProd) {
      int index = aIndex+bIndex;
      product[index] += lowerProd;
      if (isDebug) std::cout <<"\tP [" <<index <<"]  => " <<product[index] <<"\n"; 
      isProd = false;
    }
  }

  int higherProd = multiply2(product, A, B, size/2, (aIndex+size/2), (bIndex+size/2), false, isProd);
  for (int i=size; i<=(2*size-2); i++) {
    if(isProd) {
      int index = (aIndex+size/2 + bIndex+size/2);
      product[index] += higherProd;
      if (isDebug) std::cout <<"\tP [" <<index <<"]  => " <<product[index] <<"\n"; 
      isProd = false;
    } 
  }

  int midProdLeft   = multiply2(product, A, B, size/2, aIndex, (bIndex + size/2), true, isProd);
  int midProd1Right = multiply2(product, A, B, size/2, (aIndex + size/2), bIndex, false, isProd);

  unsigned count = (size + size/2 -2);
  for( int i=size/2; i <= count; i++) {
    if(isProd) {
      int index = aIndex + (bIndex + size/2);
      product[index] += (midProdLeft + midProd1Right);
      if (isDebug) std::cout <<"\t\tP [" <<index <<"]  => " <<product[index] <<"\n"; 
      isProd = false;
    }
  }
  isProd = false;
  return result;
}


int main()
{
  std::vector <int> a, b, prod;
  unsigned int degree;
  std::cout <<"######################################################################################\n";
  std::cout <<"#                       INFO                                 \n";
  std::cout <<"#    This program only works on the input which are power of 2 (so that)\n";
  std::cout <<"#    there are equal partitions for algorithm to work.\n"; 
  std::cout <<"#    Valid inputs could be: 2, 4, 8, 16, 32, 64, 128, 256, 512 ...";
  std::cout <<"#    \n#\n#  Another way to solve polynomial multiplcation would be to pad the\n";
  std::cout <<"#    the numbers with zeros to make it series length equal to next nearest \n"; 
  std::cout <<"#    multiple of 2. \n";
  std::cout <<"#    For example: plolynomail multiplcation with 3 degres may have coeef: a, b, c\n";
  std::cout <<"#    To make it work, we can always make it: 0, a, b, c\n";
  std::cout <<"#####################################################################################\n";
  std::cout <<"What's the highest degree of your polynomial: ";
  std::cin >> degree;

  // Genrate the coefficients of polynomail equation:
  generateList(a, degree, 10);
  generateList(b, degree, 15);
  showList(a);
  showList(b);
  int complexity = multiplyPolyNavie(a, b, prod);
  showList(prod);
  std::cout <<"List-Size: " <<a.size() <<" Complexity: " <<complexity <<"\n";

  bool isProd = false;

  int arrA[a.size()];
  for(int i=0; i<a.size(); i++) {arrA[i] = a[i]; }
  int arrB[b.size()];
  for(int i=0; i<b.size(); i++) {arrB[i] = b[i]; }
  int product[a.size()*2];
  for(int i=0; i<(a.size()*2); i++) {product[i]=0;}
  int prodList = multiply2(product, arrA, arrB, a.size(), 0 , 0, false, isProd);
  std::cout <<"\nPorduct of polynomail using Divide-Conquer technique: ";
  //for(int i=0; i<prod.size(); i++) {std::cout <<" " << prodList[i];} std::cout <<"\n";
  for(int i=0; i<prod.size(); i++) {std::cout <<" " << product[i];} std::cout <<"\n";
  return -1;
} 
