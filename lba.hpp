
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
    Type firstpass_plba(Type x, Type a, Type b,
              Type t0, Type loc_v, Type scale_v) {
        x = x - t0;
        Type p1 = ((b - a - x * loc_v)/a) * pnorm((b - a - x * loc_v)/(x * scale_v));
        Type p2 = ((b - x * loc_v)/a) * pnorm((b - x * loc_v)/(x * scale_v));
        Type p3 = ((x * scale_v)/a) * dnorm((b - a - x * loc_v)/(x * scale_v));
        Type p4 = ((x * scale_v)/a) * dnorm((b - x * loc_v)/(x * scale_v));
        
        return 1 + p1 - p2 + p3 - p4;
    }
    
    /*
    pdf for single response
    */
    template<class Type>
    Type firstpass_dlba(Type x, Type a, Type b,
              Type t0, Type loc_v, Type scale_v) {

        x = x - t0;
        Type p1 = -loc_v * pnorm((b - a - x * loc_v)/(x * scale_v));
        Type p2 = scale_v * dnorm((b - a - x * loc_v)/(x * scale_v));
        Type p3 = loc_v * pnorm((b - x * loc_v)/(x * scale_v));
        Type p4 = -scale_v * dnorm((b - x * loc_v)/(x * scale_v));

        return (p1 + p2 + p3 + p4)/a;
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
    vector<Type> rlba(Type a, Type b, Type t0, Type loc_v, Type scale_v) {
        return (b - runif(0, a))/rnorm(loc_v, scale_v); // reaction time
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