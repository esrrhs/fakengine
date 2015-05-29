package main

import (
	"database/sql"
	"fmt"
	_ "go-sqlite3"
	"os"
	"text/template"
	"strings"
)

type Member struct {
	Name    string
	Type string
}

type Table struct {
	Name    string
	Key string
	Members []Member
}

type Result struct {
	Name    string
	Tables []Table
}

var result = Result{}

func main() {

	if len(os.Args) < 3 {
		fmt.Println("usage:SrcFileName DstName")
		return
	}

	srcFile := os.Args[1]
	desFile := os.Args[2]

	result.Name = desFile

	if !parse(srcFile) {
		return
	}

	if !output("code_h.tpl", "code.h") {
		return
	}

	if !output("code_cpp.tpl", "code.cpp") {
		return
	}

	fmt.Println("OK")
}

func parse(filename string) bool {

	fmt.Println("open db ", filename)

	db, err := sql.Open("sqlite3", filename)
	if err != nil {
		fmt.Println(err)
	}
	defer db.Close()

	rows, err := db.Query("SELECT name FROM sqlite_master WHERE type='table' ORDER BY name;")
	if err != nil {
		fmt.Println(err)
	}

	for rows.Next() {
		var name string
		rows.Scan(&name)
		if !parse_table(filename, name) {
			return false
		}
	}

	return true
}

func parse_table(filename string, tablename string) bool {
	fmt.Println("parse table ", tablename)

	db, err := sql.Open("sqlite3", filename)
	if err != nil {
		fmt.Println(err)
	}
	defer db.Close()

	rows, err := db.Query("PRAGMA table_info(" + tablename + ")")
	if err != nil {
		fmt.Println(err)
	}

	var t Table
	t.Name = strings.Title(strings.ToLower(tablename))

	for rows.Next() {
		var id int
		var name string
		var ctype string
		var notnull int
		var dftvalue string
		var pk int
		rows.Scan(&id, &name, &ctype, &notnull, &dftvalue, &pk)
		fmt.Println(id, name, ctype)
		var m Member
		m.Name = strings.Title(strings.ToLower(name))
		m.Type = ctype
		t.Members = append(t.Members, m)
	}

	t.Key = t.Members[0].Name

	result.Tables = append(result.Tables, t)

	fmt.Println("parse table ok ", tablename)
	
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
