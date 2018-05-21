/*
**  Preprocssing
*/

#include "preproc.h"

// Grayscale
SDL_Surface* grayscale(SDL_Surface* img) {
  Uint32 pixl;
  Uint8 r, g, b;
  float avg;
  for(int i = 0; i < img->w; i++) {
    for(int j = 0; j < img->h; j++) {
      pixl = getpixel(img, i, j);
      SDL_GetRGB(pixl, img->format, &r, &g, &b);

      /* Grayscale formula */
      avg = (r*0.3) + (g*0.59) + (b*0.11);
      r = (Uint8)avg;

      putpixel(img, i, j, SDL_MapRGB(img->format, r, r, r));
    }
  }
  return img;
}

// Constrast function based on the Histogram Equalisation
SDL_Surface* contrast(SDL_Surface* img) {
  Uint32 pixl;
  Uint8 r, g, b;
  int height = img->h, width = img->w, sum = 0, max_intensity = 255;
  int histogram[256], lut[256];

  for(int i = 0; i <= max_intensity; i++)
    histogram[i] = 0;

  // We build the histogram
  for(int i = 0; i < img->w; i++) {
    for(int j = 0; j < img->h; j++) {
      pixl = getpixel(img, i, j);
      SDL_GetRGB(pixl, img->format, &r, &g, &b);

      histogram[(int)r]++;
    }
  }

  // build a LUT containing scale factor
  for (int i = 0; i <= max_intensity; i++) {
    sum += histogram[i];
    lut[i] = sum * max_intensity / (height * width);
  }

  // transform image using sum histogram as a LUT
  // We apply the changes
   for(int h = 0; height > h; h++) {
    for (int w = 0; width > w; w++) {
      SDL_GetRGB(getpixel(img,w,h),img->format,&r,&g,&b);
      putpixel(img, w, h, SDL_MapRGB(img->format, lut[r], lut[r], lut[r]));
    }
  }
  return img;
}
/*
SDL_Surface* blackAndWhite(SDL_Surface* img, int split) {
  if(!split)
    return otzu(img, 0, img->w, 0, img->h);
  int w = img->w, h = img->h, w_sp = 0, h_sp = 0;
  if(w > h) {
    w_sp = 3 * split;
    h_sp = 2 * split;
  }
  else if(w < h) {
    w_sp = 2 * split;
    h_sp = 3 * split;
  }
  else {
    w_sp = 2 * split;
    h_sp = 2 * split;
  }
  int w_r = w/w_sp, h_r = h/h_sp;
  for(int i = 0; i < w_sp; i++)
    for(int j = 0; j < h_sp; j++)
      img = otzu(img, i * w_r, (i+1) * w_r, j * h_r, (j+1) * h_r);
  return img;
}

// Otzu method for Black and White
SDL_Surface* otzu(SDL_Surface* img, int w_min, int w_max, int h_min, int h_max)
{
  Uint32 pixl;
  Uint8 r, g, b;
  int sigsq, max_intensity = 255, histogram[256];
  int N = (h_max - h_min) * (w_max - w_min); // img->h * img->w;
  int threshold=0, var_max=0, sum=0, sumB=0, q1=0, q2=0, m1=0, m2=0;

  for(int i = 0; i <= max_intensity; i++)
    histogram[i] = 0;

  // We build the histogram
  for(int i = w_min; i < w_max; i++) {
    for(int j = h_min; j < h_max; j++) {
      pixl = getpixel(img, i, j);
      SDL_GetRGB(pixl, img->format, &r, &g, &b);

      histogram[(int)r]++;
    }
  }

  // update qi(t)
  for(int t = 0; t <= max_intensity; t++) {
    sum += t * histogram[t]; // auxiliary value for computing μ2
    q1 += histogram[t];
    q2 = N - q1;
    sumB += t * histogram[t];

    if(q1 != 0)
      m1 = sumB / q1;
    if(q2 != 0)
      m2 = (sum - sumB) / q2;

    sigsq = q1 * q2 * (m1 - m2) * (m1 - m2);

    if(sigsq > var_max) {
      threshold = t;
      var_max = sigsq;
    }
  }

  // We apply the changes
  for(int h = h_min; h < h_max; h++) {
    for(int w = w_min; w < w_max; w++) {
      SDL_GetRGB(getpixel(img, w, h),img->format, &r, &g, &b);
      if(r <= threshold)
        putpixel(img, w, h, SDL_MapRGB(img->format, 0, 0, 0));
      else
        putpixel(img, w, h, SDL_MapRGB(img->format, 255, 255, 255));
    }
  }
  return img;
}*/

void binarize(SDL_Surface *img, int threshold) {
	Uint8 r;
	for (int x = 0; x < img->h; x++) {
		for (int y = 0; y < img->w; y++) {
			Uint32 pix = getpixel(img, y, x);
			SDL_GetRGB(pix, img->format, &r, &r, &r);
			Uint8 v = r < threshold ? 0 : 255;
			Uint32 npix = SDL_MapRGB(img->format, v, v, v);
			putpixel(img, y, x, npix);
		}
	}
}

/*void contrast(SDL_Surface *img, int value) {
	double factor = (259.0 * (value + 255.0)) / (255.0 * (259.0 - value));
	Uint8 r, g, b;
	for (int x = 0; x < img->h; x++) {
		for (int y = 0; y < img->w; y++) {
			Uint32 pix = getpixel(img, y, x);
			SDL_GetRGB(pix, img->format, &r, &g, &b);
			r = factor * (r - 128) + 128;
			g = factor * (g - 128) + 128;
			b = factor * (b - 128) + 128;
			Uint32 npix = SDL_MapRGB(img->format, r, g, b);
			putpixel(img, y, x, npix);
		}
	}
}*/

int otsu(SDL_Surface *img) {
	int hist[256] = {0};
	double proba[256] = {0};
	double omega[256] = {0};
	double sigma[256] = {0};
	double individual_prob = 1.0 / (img->w * img->h);
	double average_pix = 0.0;
	Uint8 r;
	for (int x = 0; x < img->w; x++) {
		for (int y = 0; y < img->h; y++) {
			Uint32 pix = getpixel(img, x, y);
			SDL_GetRGB(pix, img->format, &r, &r, &r);
			hist[r]++;
			proba[r] += individual_prob;
			average_pix += (r * individual_prob);
		}
	}
	omega[0] = proba[0];
	sigma[0] = 0.0;
	for (int i = 1; i < 256; i++) {
		omega[i] = omega[i - 1] + proba[i];
		sigma[i] = sigma[i - 1] + i * proba[i];
	}
	
	int optimal_thresh = 0;
	double sigmax = 0;
	for (int i = 0; i < 255; i++) {
		if (omega[i] != 0.0 && omega[i] != 1.0) {
			sigma[i] = pow(sigma[255] * omega[i] - sigma[i], 2) /
			(omega[i] * (1.0 - omega[i]));
		} else {
			sigma[i] = 0.0;
		}
		if (sigma[i] > sigmax) {
			sigmax = sigma[i];
			optimal_thresh = i;
		}
	}
	return optimal_thresh;
}
