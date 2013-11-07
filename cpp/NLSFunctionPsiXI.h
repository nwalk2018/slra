class NLSFunctionPsiXI : public OptFunction {
protected:
  VarproFunction &myFun;
  gsl_matrix *myTmpR;  
  gsl_matrix *myPsi;
  gsl_matrix myPsiSubm;
  gsl_matrix *myTmpXId;
  void computeR( const gsl_vector *x, gsl_matrix *R ); 
public: 
  OptFunctionSLRA( VarproFunction &fun, gsl_matrix *Psi );
  virtual ~OptFunctionSLRA();
  virtual size_t getNvar() { return getRank() * myFun.getD(); }
  virtual void computeFuncAndGrad( const gsl_vector* x, double* f, gsl_vector *grad );

  void computePhat( gsl_vector* p, const gsl_vector* x );
  
  size_t getD() { return myFun.getD(); }
  size_t getM() { return myPsi->size2; }
  size_t getRank() { return myPsi->size2 - myFun.getD(); }
  void computeDefaultx( gsl_vector *x ); 

  void RTheta2x( gsl_matrix *RTheta, gsl_vector *x ); 
  void x2RTheta( gsl_matrix *RTheta, const gsl_vector *x ); 

  gsl_matrix x2xmat( const gsl_vector *x ) {
    return gsl_matrix_const_view_vector(x, getRank(), myFun.getD()).matrix;
  }
  static void X2XId( const gsl_matrix *x, gsl_matrix *XId );
  static void PQ2XId( const gsl_matrix *PQ, gsl_matrix * x );
};

class OptFunctionSLRACholesky : public OptFunctionSLRA {
public: 
  OptFunctionSLRACholesky( VarproFunction &fun, gsl_matrix *psi ) : 
      OptFunctionSLRA(fun, psi) {}
  virtual ~OptFunctionSLRACholesky() {}
  virtual size_t getNsq() { return myFun.getN() * myFun.getD(); }
  virtual void computeFuncAndJac( const gsl_vector* x, gsl_vector *res, 
                                   gsl_matrix *jac ) {
    computeR(x, myTmpR);
    myFun.computeFuncAndPseudoJacobianLs(myTmpR, &myPsiSubm, res, jac); 
  }   
};

class OptFunctionSLRACorrection : public OptFunctionSLRA {
public: 
  OptFunctionSLRACorrection( VarproFunction &fun, gsl_matrix *psi ) : 
      OptFunctionSLRA(fun, psi)  {}
  virtual ~OptFunctionSLRACorrection() {}
  virtual size_t getNsq() { return myFun.getNp(); }
  virtual void computeFuncAndJac( const gsl_vector* x, gsl_vector *res, 
                                  gsl_matrix *jac ) {
    computeR(x, myTmpR);
    myFun.computeCorrectionAndJacobian(myTmpR, &myPsiSubm, res, jac); 
  }
};