#include<stdlib.h>
#include<stdio.h>

struct corner
{
	long x;
	long y;
	struct corner* next;
};

struct line {
	struct corner *from;
	struct corner *to;
	struct line* next;
};
	

struct corner *cornerlist = NULL;
struct line *linelist = NULL;

int linethroughbox(struct corner *left, struct corner* right, struct line* line)
{
	long boxleft   = left->x <  right->x ? left->x : right->x;
	long boxright  = left->x >= right->x ? left->x : right->x;
	long boxtop    = left->y <  right->y ? left->y : right->y;
	long boxbottom = left->y >= right->y ? left->y : right->y;
	
	long lineleft   = line->from->x <  line->to->x ? line->from->x : line->to->x;
	long lineright  = line->from->x >= line->to->x ? line->from->x : line->to->x;
	long linetop    = line->from->y <  line->to->y ? line->from->y : line->to->y;
	long linebottom = line->from->y >= line->to->y ? line->from->y : line->to->y;

	if (lineleft   >= boxright) return 0;
	if (lineright  <= boxleft) return 0;
	if (linetop    >= boxbottom) return 0;
	if (linebottom <= boxtop) return 0;	
	return 1;
}

int main(int argc, char *argv[])
{	
	long x;
	long y;
	while(scanf("%ld%*[,]%ld%*[\n]", &x, &y) != EOF) {
		struct corner *new = malloc (sizeof *new);
		new->next = cornerlist;
		new->x = x;
		new->y = y;
		cornerlist = new;
	}
	for(struct corner *c = cornerlist; c; c = c->next) {
		struct line *l = malloc (sizeof *l);
		l->from = c;
		l->to = c->next ? c->next : cornerlist;
		l->next = linelist;
		linelist = l;
	}
	

	long maxsize = 0;
	for(struct corner *left = cornerlist; left; left = left->next)
		for(struct corner *right = left->next; right; right=right->next) {
			int succes = 1;
			for(struct line *line = linelist; line && succes; line = line->next) {
				if (linethroughbox(left, right, line)) succes = 0;
			}
			if (succes) {
				long width = left->x - right->x;
				long height = left->y - right->y;
				if (width < 0) width = -width;
				if (height < 0) height = -height;
				width++;
				height++;
				long size = width*height;
				if (size > maxsize) {
					printf("Testing corners (%ld, %ld) to (%ld, %ld) : size %ld\n", 
					left->x, left->y, right->x, right->y, size);
					maxsize = size;
				}
			}
		}
	printf("%ld\n", maxsize);
}
