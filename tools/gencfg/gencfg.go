package main

import (
	"bufio"
	"fmt"
	"io"
	"os"
	"strings"
	"text/template"
)

type Member struct {
	Name    string
	Comment string
	Default string
}

type Struct struct {
	Name    string
	Comment string
	Members []Member
}

type Result struct {
	SrcName string
	Name    string
	Structs []Struct
}

var result = Result{}

func main() {

	if len(os.Args) < 3 {
		fmt.Println("usage:SrcFileName DstName")
		return
	}

	srcFile := os.Args[1]
	desFile := os.Args[2]

	fmt.Println("Read " + srcFile)
	fmt.Println("Gen " + desFile + "CfgLoader.h " + desFile + "CfgLoader.cpp")

	result.SrcName = srcFile
	result.Name = desFile

	if !parse(srcFile) {
		return
	}

	if !output("CfgLoader_h.tpl", desFile+"CfgLoader.h") {
		return
	}

	if !output("CfgLoader_cpp.tpl", desFile+"CfgLoader.cpp") {
		return
	}

	fmt.Println("OK")
}

func parse(filename string) bool {
	file, err := os.Open(filename)
	if err != nil {
		fmt.Println(err)
		return false
	}

	rd := bufio.NewReader(file)

	comment := ""

	for {
		line, err := rd.ReadString('\n')

		if err != nil || io.EOF == err {
			break
		}

		line = strings.Trim(line, "\n")
		line = strings.Trim(line, "\r")
		line = strings.TrimLeft(line, " ")
		if len(line) == 0 {
			continue
		}

		if line[0] == '#' {
			// comment
			comment = strings.TrimLeft(line, "#")
		} else if line[0] == '[' {
			end := strings.Index(line, "]")
			if end == -1 {
				fmt.Println("no ] in" + line)
				return false
			}
			name := line[1:end]

			s := Struct{Name: name, Comment: comment}

			result.Structs = append(result.Structs, s)
		} else {

			line = strings.Trim(line, " ")
			end := strings.Index(line, "=")
			if end == -1 {
				fmt.Println("no = in" + line)
				return false
			}
			name := line[0:end]
			name = strings.Trim(name, " ")

			defaultval := line[end+1:]
			defaultval = strings.Trim(defaultval, " ")

			m := Member{Name: name, Comment: comment, Default:defaultval}

			result.Structs[len(result.Structs)-1].Members = append(result.Structs[len(result.Structs)-1].Members, m)
		}
	}

	return true
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

	t = t.Funcs(template.FuncMap{})

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
