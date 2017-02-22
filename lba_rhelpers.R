# check against
# rLBA(1000, A = .5, b = 1, t0 = 0, mean_v = 2.3, sd_v = .6)
# replicate(1000, rlbala(.5, 1, 2.3, .6))
rlbala <- function(a, b, location, scale) {
    proposal <- rnorm(1, location, scale)
    if (proposal < 0) {
        while(proposal < 0) {
            proposal <- rnorm(1, location, scale)
        }
    }

    (b - runif(1, 0, a))/proposal
}

rlbala_sim_block <- function(n, number_accum, t0, a, b, location, scale) { 
    rt <- matrix(NA, nrow = n, ncol = number_accum)
    a <- rep_len(a, number_accum)
    b <- rep_len(b, number_accum)
    location <- rep_len(location, number_accum)
    scale <- rep_len(scale, number_accum)
    choice <- rep(NA, n)
    min_rt <- rep(NA, n)
    for(i in 1:n) {
        for(j in 1:number_accum) {
            rt[i,j] <- rlbala(a[j], b[j], location[j], scale[j]) + t0
        }
        min_rt[i] <- min(rt[i,])
        choice[i] <- which(rt[i,] == min(rt[i,]))
    }
    data.frame(rt = min_rt, choice = choice)
}

rlbala_sim_raw <- function(n, number_accum, t0, a, b, location, scale) { 
    rt <- matrix(NA, nrow = n, ncol = number_accum)
    a <- rep_len(a, number_accum)
    b <- rep_len(b, number_accum)
    location <- rep_len(location, number_accum)
    scale <- rep_len(scale, number_accum)
    choice <- rep(NA, n)
    min_rt <- rep(NA, n)
    for(i in 1:n) {
        for(j in 1:number_accum) {
            rt[i,j] <- rlbala(a[j], b[j], location[j], scale[j]) + t0
        }
    }
    colnames(rt) <- paste0(1:number_accum, ' t0=', t0, ',a=', a, ',b=', b,
                           ',loc=', location, ',scale=', scale)
    tidy_rt <- tidyr::gather(as.data.frame(rt), name, val)

    list(tidy_data = tidy_rt, raw_rt = rt)
}

# check against
# rtdists:::dlba_norm(rt = .3, A = .5, b = 1, t0 = 0, mean_v = 2.3, sd_v = .6)
# dlbala(.3, .5, 1, 2.3, .6)
dlbala <- function(x, a, b, location, scale) {
    term1 = (b - a - x * location)/(x * scale);
    term2 = (b - x * location)/(x * scale);
    (-location * pnorm(term1) + scale * dnorm(term1) +
     location * pnorm(term2) + -scale * dnorm(term2))/a;
}

# check against
# rtdists:::plba_norm(rt = .3, A = .5, b = 1, t0 = 0, mean_v = 2.3, sd_v = .6)
# plbala(.3, .5, 1, 2.3, .6)
plbala <- function(x, a, b, location, scale) {
    term1 = ((b - a - x * location)/a) * pnorm((b - a - x * location)/(x * scale));
    term2 = ((b - x * location)/a) * pnorm((b - x * location)/(x * scale));
    term3 = ((x * scale)/a) * dnorm((b - a - x * location)/(x * scale));
    term4 = ((x * scale)/a) * dnorm((b - x * location)/(x * scale));
    
    1 + term1 - term2 + term3 - term4;    
}

lba_log <- function()
