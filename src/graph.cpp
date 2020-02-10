#include <gvc.h>

int main()
{
	Agraph_t *graph;
	Agnode_t *from, *to;
	Agedge_t *edge;

    static GVC_t *gvc;

    if (!gvc)
    	gvc = gvContext();

	graph = agopen("g", Agdirected, nullptr);

	from = agnode(graph, "n", true);
	agsafeset(from, "shape", "circle", "");
	to = agnode(graph, "m", true);
	agsafeset(to, "shape", "circle", "");
	edge = agedge(graph, from, to, "This should be appeared", true);

	from = agnode(graph, "x", true);
	agsafeset(from, "shape", "circle", "");
	to = agnode(graph, "n", true);
	agsafeset(to, "shape", "circle", "");
	edge = agedge(graph, from, to, "This should be appeared", true);
	agsafeset(edge, "label", "circle", "");

    gvLayout(gvc, graph, "dot");

//    Output in .dot format
//    gvRender(gvc, g, "dot", stdout);
	gvRender(gvc, graph, "png", fopen("bruh.png", "w"));

    gvFreeLayout(gvc, graph);

	agclose(graph);

	return 0;
}