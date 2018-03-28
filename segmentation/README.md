# Segmentation

## ROI zones - Bonus

- [ ] find ROI
- [ ] Create ROI images
- [ ] If not found proceed to FP

## Find Finder Patterns

### Main -DONE-

- [x] Locate finder patterns (+ Draw FPs)
- [x] Check if there is 3 finder patterns at least. !
- [x] Determine FP A

### Bonus

- [ ] Check geometry !
- [ ] Determine B and C **-Not Necessairy-**
- [ ] Case of multiple QrCodes !

## Geometric Transformation

### Main 

- [x] Solve affine equations (Gauss Eliminations)
- [x] Affine transformation
- [ ] Make size-adaptive transformations

### Bonus

- [ ] Harris Corner detection
- [ ] Convex HULL
- [ ] Get new A, B, C and D
- [ ] Projection transformation 

## Etract QrCode from image

- [x] Estimate X the Width of a module
- [x] Estimate V the Version !

### Version 1

#### Main -DONE-

- [x] Set V as Version 
- [x] Estimate Y the Height of a module
- [x] Create sampling grid -> To matrix
- [x] Dynamic sampling according to col/row 1

#### Bonus

- [ ] Dynamic sampling according to timing patterns

**-> Go To Bit Stream**

### Version 2 - 6

- [ ] Set V as version

### Version 7+ 

- [ ] Decode Version and compare error

## Find Alignment patterns and Sampling

## Bit stream

### Main

- [x] Get F1 Format information
- [x] Decode Format information (Annexe C.2)
- [x] If F1 corrupt, Get F2 and restart Bit Stream
- [x] Release Mask
- [x] Retrieve Bit Stream

### Bonus

- [x] If Format not Valid, transpose x and y, and retry ! 

**-> END of Segmentation**