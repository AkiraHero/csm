#ifndef H_LASER_DATA
#define H_LASER_DATA

#include <sys/time.h>
#include <stdio.h>

#ifndef RUBY
#include <egsl/egsl.h>
#include <egsl/egsl.h>
#endif

struct correspondence;

typedef struct {
	double p[2];
	double rho, phi;
} point2d;

struct laser_data {
	int nrays;
	double  min_theta;
	double  max_theta;
	
	double * restrict theta;
	
	int    * restrict valid;
	double * restrict readings;
	
	int    * restrict cluster;
	
	double * restrict alpha;
	double * restrict cov_alpha;
	int    * restrict alpha_valid;

	double * restrict cov_readings;

	double * restrict true_alpha;
	
	/* Jump tables needed by find_correspondences_tricks(). */
	int * restrict up_bigger, 
	    * restrict up_smaller, 
	    * restrict down_bigger, 
	    * restrict down_smaller;
	
	struct correspondence*  restrict corr;

	double true_pose[3];		
	double odometry[3];	
	double estimate[3];	
	
#ifndef RUBY
	/* Cartesian points */
/*	gsl_vector**p;*/
#endif

	/** Cartesian representation */
	point2d *  restrict points;
	/** Cartesian representation, in "world" (laser_ref) coordinates. */
	point2d *  restrict points_w;

	/** Timestamp */
	struct timeval tv;
};

struct correspondence {
	/** 1 if this correspondence is valid  */
	int valid; 
	/** Closest point in the other scan.  */
	int j1; 
	/** Second closest point in the other scan.  */
	int j2;
	/** Squared distance from p(i) to point j1 */
	double dist2_j1; 
};

typedef struct laser_data* LDP;

/** This returns a new structure, with all fields initialized */
LDP ld_alloc_new(int nrays);

/** This DOES free()s the pointer  */
void ld_free(LDP);

/** This allocs the fields in the given structure. Use ld_alloc_new(), not this. */
void ld_alloc(LDP, int nrays);

/** This does NOT free the pointer. Don't use -- use ld_alloc_new()/ld_free() instead. */
void ld_dealloc(LDP);

/** Fills the "points" field */
void ld_compute_cartesian(LDP);

/** Computes the "points_w" coordinates by roto-translating "points" */
void ld_compute_world_coords(LDP, const double *pose);

/** Fills the fields: *up_bigger, *up_smaller, *down_bigger, *down_smaller.*/
void ld_create_jump_tables(LDP);

/** Computes an hash of the correspondences */
unsigned int ld_corr_hash(LDP);

#if 0
/** -1 if not found */
INLINE_DECL int ld_next_valid(LDP ld, int i, int dir);

/** True if the i-th is a valid ray */
INLINE_DECL int ld_valid_ray(LDP ld, int i);

/** True if the i-th is a valid correspondences */
INLINE_DECL int ld_valid_corr(LDP ld, int i);

/** Sets the i-th correspondence as valid. */
INLINE_DECL void ld_set_correspondence(LDP, int i, int j1, int j2);

/** Marks the i-th correspondence as invalid. */
INLINE_DECL void ld_set_null_correspondence(LDP, int i);

/** Find the next valid ray (j > i), or -1 if not found. */
INLINE_DECL int ld_next_valid_up(LDP, int i);

/** Find the prev valid ray (j < i), or -1 if not found.*/
INLINE_DECL int ld_next_valid_down(LDP, int i);
#endif

/** Returns the number of valid correspondences. */
int ld_num_valid_correspondences(LDP);

/** Do an extensive sanity check about the data contained in the structure. */
int ld_valid_fields(LDP);

/** A simple clustering algorithm. Sets the `cluster' field in the structure. */
void ld_simple_clustering(LDP ld, double threshold);

/** A cool orientation estimation algorithm. Needs cluster. */
void ld_compute_orientation(LDP ld, int size_neighbourhood, double sigma);

/** Read next FLASER line in file (initializes ld). 
	Returns 0 on success, -1 if error, -2 eof. 
	You probably want to use the ld_read_smart() function. */
int ld_read_next_laser_carmen(FILE*, LDP ld);

/** Reads all the scans it can find. */
int ld_read_all(FILE*file, LDP **array, int*num);

/** Read a scan every interval (1=all)*/
int ld_read_some_scans(FILE*file, LDP **array, int*num, int interval);

#include "laser_data_inline.h"

#endif

