package com.bcopstein.endpointsdemo1;

import java.time.LocalDate;
import java.util.ArrayList;
import java.util.List;

import jakarta.persistence.CascadeType;
import jakarta.persistence.Entity;
import jakarta.persistence.Id;
import jakarta.persistence.OneToMany;

@Entity 
public class Autor {
    @Id 
    private Long id;
    private String nome;
    private LocalDate dataNascimento;

    @OneToMany(mappedBy = "autor", cascade = CascadeType.ALL)
    private List<Livro> livros = new ArrayList<>();

    public Autor() {}

    public Autor(Long id, String nome, LocalDate dataNascimento) {
        this.id = id;
        this.nome = nome;
        this.dataNascimento = dataNascimento;
    }

    public long getId() {
        return this.id;
    }

    public String getNome() {
        return this.nome;
    }

    public LocalDate getDataNascimento() {
        return this.dataNascimento;
    }

    public List<Livro> getLivros() {
        return this.livros;
    }

    @Override 
    public String toString() {
        return "Autor[id=" + id + ", name = " + nome + ", dataNascimento = " + dataNascimento + "]";
    }
}
