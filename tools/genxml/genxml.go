package main

import (
	"encoding/xml"
	"fmt"
	"mahonia"
	"os"
	"strconv"
	"text/template"
)

type Attribute struct {
	Name  string
	Value string
	Type  string
}

type Token struct {
	Name       string
	IsArray    string
	Num        int
	Attributes map[string]*Attribute
	Tokens     map[string]*Token
}

type Result struct {
	Root Token
}

var result = Result{}
var g_tabNum = 0
var g_classVec []string

func main() {

	if !parse(os.Args[1]) {
		return
	}

	if !output("code_h.tpl", "code.h") {
		return
	}

	fmt.Println("OK")
}

func parse_token(decoder *xml.Decoder, tt xml.Token, mytoken *Token) {

	mytoken.Attributes = make(map[string]*Attribute)
	mytoken.Tokens = make(map[string]*Token)

	token := tt.(xml.StartElement)
	name := token.Name.Local
	mytoken.Name = name

	for _, attr := range token.Attr {
		attrName := attr.Name.Local
		attrValue := attr.Value

		attr := mytoken.Attributes[attrName]
		if attr == nil {
			attr = new(Attribute)
			mytoken.Attributes[attrName] = attr
		}

		attr.Name = attrName
		attr.Value = attrValue
	}

	var t xml.Token
	var err error

	for t, err = decoder.Token(); err == nil; t, err = decoder.Token() {

		switch t.(type) {
		// 处理元素开始（标签）
		case xml.StartElement:

			token := t.(xml.StartElement)
			name := token.Name.Local

			son := mytoken.Tokens[name]
			if son == nil {
				son = new(Token)
				mytoken.Tokens[name] = son
			}
			son.Num++
			parse_token(decoder, t, son)

		// 处理元素结束（标签）
		case xml.EndElement:
			return
		case xml.CharData:
		default:
		}
	}
}

func print_token(token *Token) {

	if token.Num > 1 {
		token.IsArray = "yes"
	}

	fmt.Println("token begin:", token.Name, " ", token.IsArray)

	for name, attr := range token.Attributes {
		_, err := strconv.Atoi(attr.Value)
		if err != nil {
			attr.Type = "string"
		} else {
			attr.Type = "int"
		}

		fmt.Println("attr:", name, " type ", attr.Type)
	}

	for _, e := range token.Tokens {
		print_token(e)
	}

	fmt.Println("token end:", token.Name)
}

func parse(filename string) bool {
	file, err := os.Open(filename)
	if err != nil {
		fmt.Println(err)
		return false
	}

	var t xml.Token

	decoder := xml.NewDecoder(file)
	find := false
	for t, err = decoder.Token(); err == nil && find == false; {
		switch t.(type) {
		case xml.StartElement:
			find = true
		default:
			t, err = decoder.Token()
		}
	}
	if err != nil {
		fmt.Println(err)
		return false
	}
	parse_token(decoder, t, &result.Root)

	print_token(&result.Root)

	return true
}

func genlist(n string) []string {
	num, _ := strconv.Atoi(n)
	ret := make([]string, num)
	for i := 0; i < num; i++ {
		ret[i] = strconv.Itoa(i)
	}
	return ret
}

func iconv(str string) string {
	enc := mahonia.NewEncoder("gb2312")
	return enc.ConvertString(str)
}

func add_tab() string {
	g_tabNum++
	return ""
}

func dec_tab() string {
	g_tabNum--
	return ""
}

func gen_tab() string {
	ret := ""
	for i := 0; i < g_tabNum; i++ {
		ret += "    "
	}
	return ret
}

func push_class(t string, c string) string {
	g_classVec = append(g_classVec, t+c)
	return ""
}

func pop_class() string {
	g_classVec = g_classVec[0 : len(g_classVec)-1]
	return ""
}

func gen_class() string {
	ret := ""
	for _, c := range g_classVec {
		ret += c + "::"
	}
	return ret
}

func output(src string, des string) bool {

	g_tabNum = 0

	file, err := os.Create(des)
	if err != nil {
		fmt.Println(err)
		return false
	}

	t := template.New("text")
	if err != nil {
		fmt.Println(err)
		return false
	}

	t = t.Funcs(template.FuncMap{"genlist": genlist, "iconv": iconv,
		"add_tab": add_tab, "dec_tab": dec_tab, "gen_tab": gen_tab,
		"push_class": push_class, "pop_class": pop_class, "gen_class": gen_class})

	srcfile, err := os.Open(src)
	if err != nil {
		fmt.Println(err)
		return false
	}

	var buffer [1024 * 1024]byte
	n, rerr := srcfile.Read(buffer[0:])
	if rerr != nil {
		fmt.Println(rerr)
		return false
	}

	t, err = t.Parse(string(buffer[0:n]))
	if err != nil {
		fmt.Println(err)
		return false
	}

	err = t.Execute(file, result.Root)
	if err != nil {
		fmt.Println(err)
		return false
	}

	return true
}
