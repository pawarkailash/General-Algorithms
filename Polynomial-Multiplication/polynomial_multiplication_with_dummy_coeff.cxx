#include <iostream>
#include <cstdlib>
#include <vector>
#include <math.h>

void showList(const std::vector<int> &list)
{
  std::cout <<"Coefficients of polynomial: ";
  for (int i=0; i<list.size(); i++) {
    std::cout <<" " <<list[i]; 
  } 
  std::cout <<"\n";
}


void generateList(std::vector<int> &list, unsigned int count, 
        unsigned int limit)
{
  for (int i=0; i<count; i++) {
    list.push_back(rand() % limit);
  }
}

int multiplyPolyNavie(const std::vector<int> &a, 
        const std::vector<int> &b, std::vector<int> &prod)
{
  // Initialize the product
  for (int i=0; i<(2*a.size()-1); i++) {
    prod.push_back(0);
    //prod[i] = 0;
  }
  
  unsigned count = 0;
  for (int i=0; i<a.size(); i++) {
    for (int j=0; j<a.size(); j++) {
      prod[i+j] += a[i] * b[j];
      count++;
    }
  }
  return count;
}

int multiply2(std::vector<int> &product, const std::vector<int> &A, const std::vector<int> &B, 
        unsigned size, unsigned aIndex, unsigned bIndex, bool isLow, bool &isProd)
{

  int result;
  if(size == 1) {
    result = A[aIndex] * B[bIndex];
    //std::cout <<"LOW[" <<isLow <<"]  [" <<aIndex <<"," <<bIndex <<"]  " <<result <<"\n";
    isProd = true;
    return result;    
  }

  int lowerProd = multiply2(product, A, B, size/2, aIndex, bIndex, true, isProd);
  for(int i=0; i<=(size-2); i++) {
    if(isProd) {
      //product[i] = lowerProd[0];
      int index = aIndex+bIndex;
      product[index] += lowerProd;
      //std::cout <<"\tP [" <<index <<"]  => " <<product[index] <<"\n"; 
      isProd = false;
    }
  }

  int higherProd = multiply2(product, A, B, size/2, (aIndex+size/2), (bIndex+size/2), false, isProd);
  for (int i=size; i<=(2*size-2); i++) {
    if(isProd) {
      int index = (aIndex+size/2 + bIndex+size/2);
      product[index] += higherProd;
      //std::cout <<"\tP [" <<index <<"]  => " <<product[index] <<"\n"; 
      isProd = false;
    } 
  }

  int midProdLeft   = multiply2(product, A, B, size/2, aIndex, (bIndex + size/2), true, isProd);
  int midProd1Right = multiply2(product, A, B, size/2, (aIndex + size/2), bIndex, false, isProd);

  unsigned count = (size + size/2 -2);
  //std::cout <<"Size: " <<size <<"  Count: " <<count <<"\n";
  for( int i=size/2; i <= count; i++) {
    if(isProd) {
      int index = aIndex + (bIndex + size/2);
      product[index] += (midProdLeft + midProd1Right);
      //std::cout <<"\t\tP [" <<index <<"]  => " <<product[index] <<"\n"; 
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
  std::cout <<"#    This is a polynomial multiplication program. It works any two polynomial, \n";
  std::cout <<"#    with any highest degree. It works on the fact that if highest degree supplied \n";
  std::cout <<"#    is not power of 2 then for remaining coefficients, it padds them with zeros. \n";
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
  
  unsigned power = (unsigned)ceil((double)(log(degree)/log(2.0)));
  std::cout <<"Power: " <<power <<"\n";
  unsigned radix2 = pow(2.0, power);
  std::cout <<"Radix-Power: " <<radix2 <<"\n";
  unsigned offSet = (radix2 - degree);
  
  /* Create polynomial with highest degree = radix2 and initialize it with "0" */
  std::vector<int> polynomial1(radix2, 0);
  std::vector<int> polynomial2(radix2, 0);

  /* Now copy coeffcient provided by user, in this case randomly generated */
  std::vector<int>::iterator it;
  it = polynomial1.begin() + offSet;
  std::copy(a.begin(), a.end(), it);
  showList(polynomial1);

  it = polynomial2.begin() + offSet;
  std::copy(b.begin(), b.end(), it);
  showList(polynomial2);

  bool isProd = false;

  // Size of resulting polynomial after multiplication.
  std::vector<int> productPoly((2*radix2), 0);
  
  int prodList = multiply2(productPoly, polynomial1, polynomial2, radix2, 0 , 0, false, isProd);
  std::cout <<"\nPorduct of polynomail using Divide-Conquer technique: ";
  
  /* For reporting of polynomial coeffcients, we need to remove zeros which we padded to make this
   * algorithm work. We know that we padded "offset" number of zeros in each polynomial. Since there
   * are two polynomails so result will have (offSet*2) numer of zeros, whic should be removed before
   * reporting */
  int startIndex = (offSet * 2);
  for(int i = startIndex; i<productPoly.size()-1; i++) {std::cout <<" " << productPoly[i];} std::cout <<"\n";
  return -1;
} 
