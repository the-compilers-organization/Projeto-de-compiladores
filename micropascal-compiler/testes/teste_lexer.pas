program TesteLexico;
var
  contador, Contador2, _valor : integer;
  media, pi : real;
  letra, digito, quebraLinha, tabulacao : char;

begin

  contador := 0;
  Contador2 := 1;
  _valor := 10;

  contador := 123;
  media := 3.14;
  pi := 3.14159;

  letra := 'a';
  digito := '9';
  quebraLinha := '\n';
  tabulacao := '\t';

  if contador < 10 then write('a');
  if contador > 10 then write('a');
  if contador <= 10 then write('a');
  if contador >= 10 then write('a');
  if contador = 10 then write('a');
  if contador <> 10 then write('a');

  contador := contador + 1;
  contador := contador - 1;
  contador := contador * 2;
  media := media / 2.0;
  contador := contador div 2;
  if (contador > 0) and (contador < 100) then write('a');
  if (contador > 0) or (contador < 0) then write('a');
  if not (contador = 0) then write('a');

  contador := 42;

  write(contador);

  while contador < 5 do
  begin
    contador := contador + 1;
  end;

  repeat
    contador := contador - 1;
  until contador = 0;

  if contador = 0 then
    write('z');
  else
    write('n');

end.
