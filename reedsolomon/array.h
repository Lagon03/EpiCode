struct Array{
  uint8_t *array;
  size_t used;
  size_t size;
};

void initArray(struct Array *a, size_t initialSize)
{
	a->array = malloc(sizeof(uint8_t) * initialSize);
	a->used = 0;
	a->size = initialSize;
}

void initZArray(struct Array *a, size_t initialSize)
{
	a->array = calloc(initialSize, sizeof(uint8_t));
	a->used = 0;
	a->size = initialSize;
}

void insertArray(struct Array *a)
{
	if(a->used == a->size)
	{
		a->size *= 2;
		a->array = realloc(a->array, a->size * sizeof(uint8_t));
	}
	a->used++;
}

void deleteArray(struct Array *a)
{
	a->used--;
}


void freeArray(struct Array *a)
{
	free(a->array);
	a->array = NULL;
	a->used = 0;
	a->size = 0;
}