/*
   ARPACK++ v1.0 8/1/1997
   c++ interface to ARPACK code.

   MODULE ARDSSym.h.
   Arpack++ class ARluSymStdEig definition
   (dense matrix version).

   ARPACK Authors
      Richard Lehoucq
      Danny Sorensen
      Chao Yang
      Dept. of Computational & Applied Mathematics
      Rice University
      Houston, Texas
*/

#ifndef ARDSSYM_H
#define ARDSSYM_H

#include <stddef.h>
#include "arch.h"
#include "arssym.h"
#include "ardsmat.h"


template<class FLOAT>
class ARluSymStdEig:
  public virtual ARSymStdEig<FLOAT, ARdsSymMatrix<FLOAT> > {

 public:

 // a) Public functions:

 // a.1) Functions that allow changes in problem parameters.

  virtual void ChangeShift(FLOAT sigmaRp);

  virtual void SetRegularMode();

  virtual void SetShiftInvertMode(FLOAT sigmap);

 // a.2) Constructors and destructor.

  ARluSymStdEig() { }
  // Short constructor.

  ARluSymStdEig(int nevp, ARdsSymMatrix<FLOAT>& A,
                char* whichp = "LM", int ncvp = 0,
                FLOAT tolp = 0.0, int maxitp = 0,
                FLOAT* residp = NULL, bool ishiftp = true);
  // Long constructor (regular mode).

  ARluSymStdEig(int nevp, ARdsSymMatrix<FLOAT>& A,
                FLOAT sigma, char* whichp = "LM", int ncvp = 0,
                FLOAT tolp = 0.0, int maxitp = 0,
                FLOAT* residp = NULL, bool ishiftp = true);
  // Long constructor (shift and invert mode).

  ARluSymStdEig(const ARluSymStdEig& other) { Copy(other); }
  // Copy constructor.

  virtual ~ARluSymStdEig() { }
  // Destructor.

 // b) Operators.

  ARluSymStdEig& operator=(const ARluSymStdEig& other);
  // Assignment operator.

}; // class ARluSymStdEig.


// ------------------------------------------------------------------------ //
// ARluSymStdEig member functions definition.                               //
// ------------------------------------------------------------------------ //


template<class FLOAT>
inline void ARluSymStdEig<FLOAT>::
ChangeShift(FLOAT sigmaRp)
{

  sigmaR    = sigmaRp;
  sigmaI    = 0.0;
  mode      = 3;
  iparam[7] = mode;

  objOP->FactorAsI(sigmaR);
  Restart();

} // ChangeShift.


template<class FLOAT>
inline void ARluSymStdEig<FLOAT>::SetRegularMode()
{

  ARStdEig<FLOAT, FLOAT, ARdsSymMatrix<FLOAT> >::
    SetRegularMode(objOP, &ARdsSymMatrix<FLOAT>::MultMv);

} // SetRegularMode.


template<class FLOAT>
inline void ARluSymStdEig<FLOAT>::SetShiftInvertMode(FLOAT sigmap)
{

  ARStdEig<FLOAT, FLOAT, ARdsSymMatrix<FLOAT> >::
    SetShiftInvertMode(sigmap, objOP, &ARdsSymMatrix<FLOAT>::MultInvv);

} // SetShiftInvertMode.


template<class FLOAT>
inline ARluSymStdEig<FLOAT>::
ARluSymStdEig(int nevp, ARdsSymMatrix<FLOAT>& A,
              char* whichp, int ncvp, FLOAT tolp,
              int maxitp, FLOAT* residp, bool ishiftp)
{

  NoShift();
  // MOD-BY-LEETEN 2015/01/20:	DefineParameters(A.ncols(), nevp, &A, ARdsSymMatrix<FLOAT>::MultMv,
  DefineParameters(A.ncols(), nevp, &A, &ARdsSymMatrix<FLOAT>::MultMv,
  // MOD-BY-LEETEN 2015/01/20-END
                   whichp, ncvp, tolp, maxitp, residp, ishiftp);

} // Long constructor (regular mode).


template<class FLOAT>
inline ARluSymStdEig<FLOAT>::
ARluSymStdEig(int nevp, ARdsSymMatrix<FLOAT>& A,
              FLOAT sigmap, char* whichp, int ncvp, FLOAT tolp,
              int maxitp, FLOAT* residp, bool ishiftp)

{

  DefineParameters(A.ncols(), nevp, &A, &ARdsSymMatrix<FLOAT>::MultInvv,
                   whichp, ncvp, tolp, maxitp, residp, ishiftp);
  ChangeShift(sigmap);

} // Long constructor (shift and invert mode).


template<class FLOAT>
ARluSymStdEig<FLOAT>& ARluSymStdEig<FLOAT>::
operator=(const ARluSymStdEig<FLOAT>& other)
{

  if (this != &other) { // Stroustrup suggestion.
    ClearMem();
    Copy(other);
  }
  return *this;

} // operator=.


#endif // ARDSSYM_H

