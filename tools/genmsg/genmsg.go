package main

import (
	"encoding/xml"
	"fmt"
	"os"
	"text/template"
)

type Member struct {
	Name    string `xml:"name,attr"`
	Type    string `xml:"type,attr"`
	Comment string `xml:"comment,attr"`
	Length  string `xml:"length,attr"`
	Ref     string `xml:"ref,attr"`
}

type Struct struct {
	Name    string   `xml:"name,attr"`
	Type    string   `xml:"type,attr"`
	Comment string   `xml:"comment,attr"`
	Members []Member `xml:"member"`
}

type Enum struct {
	Name    string   `xml:"name,attr"`
	Comment string   `xml:"comment,attr"`
	Members []Member `xml:"member"`
}

type Result struct {
	XMLName xml.Name `xml:"result"`
	Structs []Struct `xml:"struct"`
	Enums   []Enum   `xml:"enum"`
}

var result = Result{}

func main() {
	if len(os.Args) < 4 {
		fmt.Println("usage: genmsg xmlfile srcfile destfile")
		return
	}

	fmt.Println("xmlfile:", os.Args[1])
	fmt.Println("srcfile:", os.Args[2])
	fmt.Println("destfile:", os.Args[3])

	if !parse(os.Args[1]) {
		fmt.Println("parse error", os.Args[1])
		return
	}
	if !output(os.Args[3], os.Args[2]) {
		fmt.Println("output error", os.Args[2], os.Args[3])
		return
	}
	fmt.Println("OK")
}

func parse(src string) bool {
	data, err := os.ReadFile(src)
	if err != nil {
		fmt.Println(err)
		return false
	}
	if err := xml.Unmarshal(data, &result); err != nil {
		fmt.Println(err)
		return false
	}
	return true
}

func getTypeSize(str string) string {
	switch str {
	case "int8_t", "uint8_t":
		return "1"
	case "int16_t", "uint16_t":
		return "2"
	case "int32_t", "uint32_t", "float":
		return "4"
	case "int64_t", "uint64_t":
		return "8"
	default:
		return "unknow"
	}
}

func isNormalType(str string) string {
	switch str {
	case "int8_t", "uint8_t",
		"int16_t", "uint16_t",
		"int32_t", "uint32_t",
		"int64_t", "uint64_t",
		"float":
		return "yes"
	default:
		return ""
	}
}

func output(dest, src string) bool {
	tpl, err := os.ReadFile(src)
	if err != nil {
		fmt.Println(err)
		return false
	}

	t, err := template.New(src).Funcs(template.FuncMap{
		"is_normal_type": isNormalType,
		"get_type_size":  getTypeSize,
	}).Parse(string(tpl))
	if err != nil {
		fmt.Println(err)
		return false
	}

	file, err := os.Create(dest)
	if err != nil {
		fmt.Println(err)
		return false
	}
	defer file.Close()

	if err := t.Execute(file, result); err != nil {
		fmt.Println(err)
		return false
	}
	return true
}
