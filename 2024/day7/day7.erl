-module(day7).
-export([solve/0]).

readlines(FileName) ->
    {ok, Device} = file:open(FileName, [read]),
    try
        get_all_lines(Device)
    after
        file:close(Device)
    end.

get_all_lines(Device) ->
    case io:get_line(Device, "") of
        eof -> [];
        Line -> Line ++ get_all_lines(Device)
    end.

parseLine(Line) ->
    [Val1, Val2] = re:split(Line, ":"),
    { binary_to_integer(Val1), [ binary_to_integer(Val) || Val <- re:split(Val2, " "), byte_size(Val) > 0]}.

insertToList([], List) -> List;
insertToList([{K,V}|T], List) -> insertToList(T, lists:append([{K,V}], List)).

getInput() -> insertToList([ parseLine(binary_to_list(Line)) || Line <- re:split(readlines("input.txt"), "\n") ], []).

add(A,B) ->  A+B.
mult(A,B) ->  A*B.

aux([H], Vals) -> [Op(H,Val) ||  Val <- Vals, Op <- [fun add/2,fun mult/2]];
aux([H|T], []) -> aux(T, [H]);
aux([H|T], Vals) -> aux(T, [Op(H,Val) ||  Val <- Vals, Op <- [fun add/2, fun mult/2]]).

checkLine(Key, Nums) -> 
    Values = aux(Nums, []),
    case lists:search(fun(T) -> Key == T end, Values) of
        {value, _} -> Key;
        _ -> 0
    end.

challenge1(Input) ->
    lists:sum([checkLine(Key, Nums) || {Key, Nums} <- Input]).

concat2(A,B) ->  list_to_integer(string:concat(integer_to_list(A), integer_to_list(B))).

aux2([H], Vals) -> [Op(Val,H) ||  Val <- Vals, Op <- [fun concat2/2, fun add/2, fun mult/2]];
aux2([H|T], []) -> aux2(T, [H]);
aux2([H|T], Vals) -> aux2(T, [Op(Val,H) ||  Val <- Vals, Op <- [fun concat2/2, fun add/2, fun mult/2]]).

checkLine2(Key, Nums) -> 
    Values = aux2(Nums, []),
    case lists:search(fun(T) -> Key == T end, Values) of
        {value, _} -> Key;
        _ -> 0
    end.

challenge2(Input) ->
    lists:sum([checkLine2(Key, Nums) || {Key, Nums} <- Input]).

solve() ->
    Input = getInput(),
    % Input = [{12, [1,2]}, {2, [1,2]}, {3,[1,2]}],
    io:format("Challenge1: ~p\n", [challenge1(Input)]),
    io:format("Challenge2: ~p\n", [challenge2(Input)]).