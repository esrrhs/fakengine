package main

import (
	"encoding/xml"
	"fmt"
	"mahonia"
	"os"
	"strconv"
	"text/template"
)

type Member struct {
	Name    string `xml:"name,attr"`
	Comment string `xml:"comment,attr"`
	Cmp     string `xml:"cmp,attr"`
}

type Array struct {
	Name    string `xml:"name,attr"`
	Comment string `xml:"comment,attr"`
	Length  string `xml:"length,attr"`
}

type TopArray struct {
	Name    string `xml:"name,attr"`
	Comment string `xml:"comment,attr"`
	Length  string `xml:"length,attr"`
	Top     string `xml:"top,attr"`
	Cmp     string `xml:"cmp,attr"`
}

type HashMap struct {
	Name    string `xml:"name,attr"`
	Comment string `xml:"comment,attr"`
	Length  string `xml:"length,attr"`
	Top     string `xml:"top,attr"`
	Cmp     string `xml:"cmp,attr"`
}

type Struct struct {
	Name     string    `xml:"name,attr"`
	Comment  string    `xml:"comment,attr"`
	Members  []Member  `xml:"member"`
	Arrays   []Array   `xml:"array"`
	TopArrays   []TopArray   `xml:"toparray"`
	HashMaps []HashMap `xml:"hashmap"`
}

type Result struct {
	XMLName xml.Name `xml:"result"`
	Structs []Struct `xml:"struct"`
}

var result = Result{}

func main() {

	if !parse() {
		return
	}

	if !output("StatMng_h.tpl", "StatMng.h") {
		return
	}

	fmt.Println("OK")
}

func parse() bool {
	file, err := os.Open("stat.xml")
	if err != nil {
		fmt.Println(err)
		return false
	}

	var buffer [1024 * 1024]byte
	n, rerr := file.Read(buffer[0:])
	if rerr != nil {
		fmt.Println(rerr)
		return false
	}

	err = xml.Unmarshal(buffer[0:n], &result)
	if err != nil {
		fmt.Println(err)
		return false
	}

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

func output(src string, des string) bool {

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

	t = t.Funcs(template.FuncMap{"genlist": genlist, "iconv": iconv})

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

	err = t.Execute(file, result.Structs)
	if err != nil {
		fmt.Println(err)
		return false
	}

	return true
}
