#Commands

 - `exit`
 program termination
 - `clear`
 clear the previous information from the grid
 - `neighb <row> <col>`
 display the neighbors of cell on row <row> and column <col>
 - `neighb_knight <row> <col>`
 displays the neighbors according to the knights steps in chess
 - `bfs <row> <col>`
 execute the BFS traversal, starting from the cell on row <row> and column <col>
 - `bfs_step <row> <col>`
 same as above, but the result is displayed step by step, depending on the distance from the source node
 - `bfs_knight <row> <col>`
  same as bfs, but it uses neighb_knight
 - `bfs_knight_step <row> <col>`
 same as above, but the result is displayed step by step, depending on the distance from the source node
 - `bfs_tree <row> <col>`
 same as bfs, but it will also display the output tree under the grid
 - `path <row1> <col1> <row2> <col2>`
 displays the shortest path between (<row1> <col1>) and (<row2> <col2>)
 - `perf`
 generates the charts for the algorithm evaluation
