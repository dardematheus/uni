package com.bcopstein.endpointsdemo1;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;

import jakarta.persistence.Entity;
import jakarta.persistence.Id;
import jakarta.persistence.JoinColumn;
import jakarta.persistence.ManyToOne;
import jakarta.persistence.JoinColumn;

@Entity
public class Livro {
    @Id
    private int id;
    private String titulo;
    private int ano;

    @ManyToOne
    @JoinColumn(name = "autor_id")
    @JsonIgnoreProperties("livros")
    private Autor autor;

    public Livro() {}

    public Livro(int id, String titulo, Autor autor, int ano) {
        this.id = id;
        this.titulo = titulo;
        this.autor = autor;
        this.ano = ano;
    }

    public int getId() {
        return this.id;
    }

    public String getTitulo() {
        return this.titulo;
    }

    public Autor getAutor() {
        return this.autor;
    }

    public int getAno() {
        return this.ano;
    }
}
