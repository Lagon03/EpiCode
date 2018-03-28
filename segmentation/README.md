# Segmentation

## ROI zones

- [ ] find ROI
- [ ] Create ROI images

## Find Finder Patterns

### Main

- [x] Locate finder patterns (+ Draw FPs)
- [x] Check if there is 3 finder patterns at least.
- [x] Determine FP A

### Bonus

- [ ] Check geometry
- [ ] Determine B and C
- [ ] Case of multiple QrCodes

## Geometric Transformation

### Main

- [x] solve affine equations (Gauss Eliminations)
- [x] affine transformation
- [ ] Make size-adaptive transformations

### Bonus

- [ ] Harris Corner detection
- [ ] Convex HULL
- [ ] Get new A, B, C and D
- [ ] projection transformation 

## Etract QrCode from image

- [x] estimate X the Width of a module
- [x] estimate V the Version

### Version 1

#### Main

- [x] set V as Version
- [x] estimate Y the Height of a module
- [x] Create sampling grid -> To matrix
- [x] Dynamic sampling according to col/row 1

**-> Go to bit stream**

#### Bonus

- [ ] Dynamic sampling according to timing patterns


### Version 2 - 6

#### Main

- [ ] set estimated version as version

### Version 7+ 

- [ ] Decode Version and compare error

## Find Alignment patterns

## Image sampling

## Bit stream
