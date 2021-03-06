#ifndef _EMBEDDEDNF_H
#define _EMBEDDEDNF_H

//Use a 32-bit DFT.  It won't work for AVRs, but for any 32-bit systems where
//they can multiply quickly, this is the bees knees.
#define USE_32DFT

#ifndef DFREQ
#define DFREQ     8000
#endif

#define BASE_FREQ 55.0  // You may make this a float.

//The higher the number the slackier your FFT will be come.
#define FUZZ_IIR_BITS  1

//Notes are the individually identifiable notes we receive from the sound.
//We track up to this many at one time.  Just because a note may appear to
//vaporize in one frame doesn't mean it is annihilated immediately.
#define MAXNOTES  12

//We take the raw signal off of the 
#define FILTER_BLUR_PASSES 2

//Determines bit shifts for where notes lie.  We represent notes with an
//uint8_t.  We have to define all of the possible locations on the note line
//in this. note_frequency = 0..((1<<SEMIBITSPERBIN)*FIXBPERO-1)
#define SEMIBITSPERBIN 3 
#define NOTERANGE ((1<<SEMIBITSPERBIN)*FIXBPERO)


//If there is detected note this far away from an established note, we will
//then consider this new note the same one as last time, and move the
//established note.  This is also used when combining notes.  It is this
//distance times two.
#define MAX_JUMP_DISTANCE 4

#define MAX_COMBINE_DISTANCE 7


//These control how quickly the IIR for the note strengths respond.  AMP 1 is
//the response for the slow-response, or what we use to determine size of
//splotches, AMP 2 is the quick response, or what we use to see the visual
//strength of the notes.
#define AMP_1_IIR_BITS 5
#define AMP_2_IIR_BITS 2

//This is the amplitude, coming from folded_bins.  If the value is below this
//it is considered a non-note.
#define MIN_AMP_FOR_NOTE 80

//If the strength of a note falls below this, the note will disappear, and be
//recycled back into the unused list of notes.
#define MINIMUM_AMP_FOR_NOTE_TO_DISAPPEAR 64



#ifdef USE_32DFT
#include "DFT32.h"
#else
#include "dft.h"
#endif

extern uint16_t folded_bins[]; //[FIXBPERO] <- The folded fourier output.
extern uint16_t fuzzed_bins[]; //[FIXBINS]  <- The Full DFT after IIR, Blur and Taper

//frequency of note; Note if it is == 255, then it means it is not set. It is
//generally a value from 
extern uint8_t  note_peak_freqs[];
extern uint16_t note_peak_amps[];  //[MAXNOTES] 
extern uint16_t note_peak_amps2[];  //[MAXNOTES]  (Responds quicker)
extern uint8_t  note_jumped_to[]; //[MAXNOTES] When a note combines into another one,
	//this records where it went.  I.e. if your note just disappeared, check this flag.

void Init();
void UpdateFreqs();
void HandleFrameInfo();



#endif

