
namespace lba {
    /* 
      cdf/pdf for single accumulator/response
      only takes scalars!
      implementing non-truncated version first
      This isn't super efficient, but the aim is to be
      clearer than the existing reference implementations...
      logs only too -- do I just exp() it?
    */
    template<class Type>
    Type firstpass_plba_cdf(Type x, Type a, Type b,
                            Type loc_v, Type scale_v) {


        Type term1 = ((b - a - x * loc_v)/a) * pnorm((b - a - x * loc_v)/(x * scale_v));
        Type term2 = ((b - x * loc_v)/a) * pnorm((b - x * loc_v)/(x * scale_v));
        Type term3 = ((x * scale_v)/a) * dnorm((b - a - x * loc_v)/(x * scale_v));
        Type term4 = ((x * scale_v)/a) * dnorm((b - x * loc_v)/(x * scale_v));
        
        return 1 + p1 - p2 + p3 - p4;
    }
    
    /*
    pdf for single response
    */
    template<class Type>
    Type firstpass_dlba_pdf(Type x, Type a, Type b,
                            Type loc_v, Type scale_v) {

        Type term1 = (b - a - x * loc_v)/(x * scale_v);
        Type term2 = (b - x * loc_v)/(x * scale_v);
        return (-loc_v * pnorm(term1) + scale_v * dnorm(term_1) +
                loc_v * pnorm(term2) + -scale_v * dnorm(term_2))/a;
    }
    /*
    Defective density function for multi reponses
    */
    template<class Type>
    Type dlba_defective(Type x, Type response, Type a, Type b,
                        Type t0, Type loc_v, Type scale_v, 
                        Type num_accum) {
        Type logres;


    }
    // only gives termination time for a single trial/single accumulator
    // (left up to the reader to determine choice/correctness)
    // see msm:::rtnorm for simulating from truncated normal
    // (or others)
    template<class Type>
    vector<Type> rlba(Type a, Type b, Type loc_v, Type scale_v) {
        Type rand_proposal = rnorm(loc_v, scale_v);
        if (rand_proposal < 0) {
            while (rand_proposal < 0) {
                rand_proposal = rnorm(loc_v, scale_v);
            }
        }
        return (b - runif(0, a))/rand_proposal; // reaction time
    }

    template<class Type>
    vector<Type> rlba_wrap(vector<Type> a, vector<Type> b,
                           Type t0, vector<Type> loc_v, 
                           vector<Type> scale_v, Type num_accum) {
        vector<Type> result; // return termination time and choice
        vector<Type> times;
        int ii;
        for(ii = 0; ii < num_accum - 1; ii++) {
            times[ii] = rlba(a[ii], b[ii], t0, loc_v[ii], scale_v[ii]);
        }
    }

}