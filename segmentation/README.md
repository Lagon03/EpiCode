# Segmentation

## ROI zones - Bonus

- [ ] find ROI
- [ ] Create ROI images
- [ ] If not found proceed to FP

## Find Finder Patterns

### Main -DONE-

- [x] Locate finder patterns
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

### Bonus - Going Further

- [ ] Harris Corner detection
- [ ] Convex HULL
- [ ] Get new A, B, C and D
- [ ] Projection transformation 

## Extract QrCode from image

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

- [x] Set V as version

**->Go To AP and Sampling**

### Version 7+ 

- [x] Decode Version and compare error

## Find Alignment patterns and Sampling

- [x] Determine Central Coordinate of each AP
- [x] Scan each AP
- [ ] Upgrade AP Scan
- [x] Sample Between each AP

## Bit stream -DONE-

### Main -DONE-

- [x] Get F1 Format information
- [x] Decode Format information (Annexe C.2)
- [x] If F1 corrupt, Get F2 and restart Bit Stream
- [x] Release Mask
- [x] Retrieve Bit Stream

### Bonus -DONE-

- [x] If Format not Valid, transpose matrix, and retry ! 

**-> END of Segmentation**
