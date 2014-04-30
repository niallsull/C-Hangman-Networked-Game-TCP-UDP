C-Hangman-Networked-Game-TCP-UDP
================================

College assignment involving modifying an existing TCP Hangman game to work better, and creating a UDP version



State of the game is held on the server side

With tcp we fork to handle multiple clients and keep the connection

UDP there  is no state on the connection, so I have structs for the state of games, server just accepts in the request and deals with iteratively, it won't take it long to deal with a single request, so we can get away with an single process iterative approach
