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
		fmt.Println("param: xmlfile srcfile destfile")
		return
	}

	fmt.Println("xmlfile:", os.Args[1])
	fmt.Println("srcfile:", os.Args[2])
	fmt.Println("destfile:", os.Args[3])

	if !parse(os.Args[1]) {
		fmt.Println("parse error ", os.Args[1])
		return
	}

	if !output(os.Args[3], os.Args[2]) {
		fmt.Println("output error ", os.Args[2], " ", os.Args[3])
		return
	}

	fmt.Println("OK")
}

func parse(src string) bool {
	file, err := os.Open(src)
	if err != nil {
		fmt.Print(err)
		return false
	}

	var buffer [102400]byte
	n, rerr := file.Read(buffer[0:])
	if rerr != nil {
		fmt.Print(rerr)
		return false
	}

	err = xml.Unmarshal(buffer[0:n], &result)
	if err != nil {
		fmt.Print(err)
		return false
	}

	return true
}

func get_type_size(str string) string {
	if str == "int8_t" ||
		str == "uint8_t" {
		return "1"
	}
	if str == "int16_t" ||
		str == "uint16_t" {
		return "2"
	}
	if str == "int32_t" ||
		str == "uint32_t" ||
		str == "float" {
		return "4"
	}
	if str == "int64_t" ||
		str == "uint64_t" {
		return "8"
	}
	return "unknow"
}

func is_normal_type(str string) string {
	if str == "int8_t" ||
		str == "uint8_t" ||
		str == "int16_t" ||
		str == "uint16_t" ||
		str == "int32_t" ||
		str == "uint32_t" ||
		str == "int64_t" ||
		str == "uint64_t" ||
		str == "float" {
		return "yes"
	}
	return ""
}

func output(dest string, src string) bool {

	file, err := os.Create(dest)
	if err != nil {
		fmt.Println(err)
		return false
	}

	t := template.New("text")
	if err != nil {
		fmt.Println(err)
		return false
	}

	t = t.Funcs(template.FuncMap{"is_normal_type": is_normal_type,
		"get_type_size": get_type_size})

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

	err = t.Execute(file, result)
	if err != nil {
		fmt.Println(err)
		return false
	}

	return true
}
