(* Test scanner and parser *)
open Ast

let _ =
  let lexbuf = Lexing.from_channel stdin in
  let program = Skanparse.program Scanner.token lexbuf in
  print_endline (string_of_program program)
