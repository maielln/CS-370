// as of right now all lines were copied from the original pascal file

{ This unit contains functions useful for dealing with filenames. }

{$G+}{$N+}{$E+}{$X+}{$D-}{$S-}{$V-}
Unit fileLib;

INTERFACE

Uses
  Dos;


  Function EXIST(thisfile : pathstr) : boolean;
  Function VALID(thisfile : pathstr) : boolean;
  Function name_form(name:string):string;
  function exten(name:string):string;
  function base_name(name:string):string;
  Function attribs(b:byte):string;
  function path(fn:string):string;
  function no_path(fn:string):string;
  function file_length(fn:string):longint;

IMPLEMENTATION

Const
  null  = #0;
  bell  = #7;
  esc   = #27;
  f10   = #$44; {scan code}
  basex : byte = 1;
  basey : byte = 1;
  tempx : byte = 1;
  tempy : byte = 1;
  endx  : byte = 24;
  endy  : byte = 80;

Var
  regs        : registers;
  textattr    : word;
  workstr     : string;


Function addfront(b:string;l:integer): string; //function that takes in a string (b) and an integer (L)
Begin
  while length(b)< l do //length of b < L
    b := ' '+b; //if b = hi, then it adds a space in front of whatever is b, (i.e.: " hi" for 1 loop
  addfront := b; //returns b
End;

Function addrear(b:string;l:integer): string; //function that takes in a string (b) and an integer (L)
Begin
  while length(b)< l do //length of b < L
    b := b+' '; //if b = hi, then it adds a space behind whatever b is, (i.e.: "hi " for 1 loop
  addrear := b; //returns b
End;

function lstr(s1:string; l:integer):string; //function that takes in a string (s1) and an integer (L)
begin
 if length(s1)<=l then lstr:=s1 //checks if the length of s1 is greater than L, if it is then it returns s1
 else lstr:=copy(s1,1,l); //copy is a pascal function in the library, link to its use: https://www.freepascal.org/docs-html/rtl/system/copy.html
end;

function rstr(s1:string; l:integer):string;
begin
 if length(s1)<=l then rstr:=s1
 else rstr:=copy(s1,length(s1)-l+1,l);
end;


Function EXIST(thisfile : pathstr) : boolean;
  var
    afile : file;
    iocode : word;

  begin {* fExist *}
    assign(afile,thisfile);
    {$I-}
    reset(afile);
    iocode := ioresult;
    {$I+}
    Exist := (iocode = 0);
    if iocode = 0 then close(afile);
  end;  {* fExist *}

Function VALID(thisfile : pathstr) : boolean;
  Var
    afile : file;
    check : boolean;
    iocode : word;

  begin {* fValid *}
    if not Exist(thisfile) then
      begin
        assign(afile,thisfile);
        {$I-}
        rewrite(afile);
        close(afile);
        erase(afile);
        {$I+}
        iocode := ioresult;
        Valid := (iocode = 0)
      end
    else Valid := true
  end;  {* fValid *}

Function name_form(name:string):string;
var
 i,j,k,l:integer;
 s1,s2,s3:string;
begin
 s1:=''; s2:='';
 k:=1;
 if (name='.') or (name='..') then
  begin
   name_form:=addrear(name,12);
   exit;
  end;
 while (k<=length(name)) and (name[k]<>'.') do
  begin
   s1:=s1+name[k];
   inc(k);
  end;
 if k<length(name) then
  begin
   inc(k);
   while (k<=length(name)) and (name[k]<>'.') do
    begin
     s2:=s2+name[k];
     inc(k);
    end;
  end;
 name_form:=addrear(s1,9)+addrear(s2,3);
end;

function exten(name:string):string;
var
 i,j,k,l:integer;
 s1,s2,s3:string;
begin
 s1:=''; s2:='';
 k:=1;
 while (k<=length(name)) and (name[k]<>'.') do
  begin
   s1:=s1+name[k];
   inc(k);
  end;
 if k<length(name) then
  begin
   inc(k);
   while (k<=length(name)) and (name[k]<>'.') do
    begin
     s2:=s2+name[k];
     inc(k);
    end;
  end;
 exten:=addrear(s2,3);
end;


function base_name(name:string):string;
var
 i,j,k,l:integer;
 s1,s2,s3:string;
begin
 s1:=''; s2:='';
 k:=1;
 while (k<=length(name)) and (name[k]<>'.') do
  begin
   s1:=s1+name[k];
   inc(k);
  end;
 base_name:=s1;
end;

Function attribs(b:byte):string;
{const
   ReadOnly = $01;
   Hidden   = $02;
   SysFile  = $04;
   VolumeID = $08;
   Directory= $10;
   Archive  = $20;
   AnyFile  = $3F;}
var
s1:string[8];
begin
 s1:=' ';
 if (b and readonly)<>0  then s1:=s1+'R' else s1:=s1+'.';
 if (b and hidden  )<>0  then s1:=s1+'H' else s1:=s1+'.';
 if (b and sysfile )<>0  then s1:=s1+'S' else s1:=s1+'.';
 if (b and archive )<>0  then s1:=s1+'A' else s1:=s1+'.';
 attribs:=s1;
end;

function path(fn:string):string;
var
 i,k:integer;
begin
 k:=0;
 for i:=length(fn) downto 1 do
  begin
   if ((fn[i]='\') or (fn[i]=':')) and (k<i) then k:=i;
  end;
 if k<>0 then
   path:=lstr(fn,k)
  else
   path:='';
end;

function no_path(fn:string):string;
var
 i,k:integer;
begin
 k:=0;
 for i:=length(fn) downto 1 do
  begin
   if ((fn[i]='\') or (fn[i]=':')) and (k<i) then k:=i;
  end;
 if k<>0 then
   no_path:=rstr(fn,length(fn)-k)
  else
   no_path:=fn;
end;

function file_length(fn:string):longint;
var
 sr:searchrec;
begin
 findfirst(fn,archive,sr);
 if doserror=0 then file_length:=sr.size
               else file_length:=0;
end;

end. Unit
