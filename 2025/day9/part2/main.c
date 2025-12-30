#include<stdlib.h>
#include<stdio.h>

// list of corners
struct corner
{
	long x;
	long y;
	struct corner* next;
};

// list of lines, connecting corners
struct line {
	struct corner *from;
	struct corner *to;
	struct line* next;
};
	

// root of lists
struct corner *cornerlist = NULL;
struct line *linelist = NULL;

// test if line crosses area between two corners
int linethroughbox(struct corner *left, struct corner* right, struct line* line)
{
	// normalize / order points of corners
	long boxleft   = left->x <  right->x ? left->x : right->x;
	long boxright  = left->x >= right->x ? left->x : right->x;
	long boxtop    = left->y <  right->y ? left->y : right->y;
	long boxbottom = left->y >= right->y ? left->y : right->y;
	
	// normalize / order points of lines
	long lineleft   = line->from->x <  line->to->x ? line->from->x : line->to->x;
	long lineright  = line->from->x >= line->to->x ? line->from->x : line->to->x;
	long linetop    = line->from->y <  line->to->y ? line->from->y : line->to->y;
	long linebottom = line->from->y >= line->to->y ? line->from->y : line->to->y;

	// return 0 when when line not in box
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
	// Smart scanf to positions of corners
	while(scanf("%ld%*[,]%ld%*[\n]", &x, &y) != EOF) {

		// Build linked list
		struct corner *new = malloc (sizeof *new);
		new->next = cornerlist;
		new->x = x;
		new->y = y;
		cornerlist = new;
	}

	// construct line segments between corners
	for(struct corner *c = cornerlist; c; c = c->next) {
		struct line *l = malloc (sizeof *l);
		l->from = c;
		l->to = c->next ? c->next : cornerlist;
		l->next = linelist;
		linelist = l;
	}
	
	long maxsize = 0;
	// brute-force it: try every combination of corners
	for(struct corner *left = cornerlist; left; left = left->next)
		for(struct corner *right = left->next; right; right=right->next) {
			int succes = 1;
			// Check is any of the lines crosses the box (break early)
			for(struct line *line = linelist; line && succes; line = line->next) {
				if (linethroughbox(left, right, line))
					succes = 0;
			}
			if (succes) {
				// Calculate surface
				long width = left->x - right->x;
				long height = left->y - right->y;
				if (width < 0) width = -width;
				if (height < 0) height = -height;

				// Increase by one (border counts as well)
				width++;
				height++;

				// Find largest area
				long size = width*height;
				if (size > maxsize) {
					maxsize = size;
				}
			}
		}
	printf("%ld\n", maxsize);
}
