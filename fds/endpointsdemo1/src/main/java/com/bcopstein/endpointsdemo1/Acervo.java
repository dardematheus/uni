package com.bcopstein.endpointsdemo1;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.springframework.stereotype.Repository;

import jakarta.annotation.PostConstruct;

@Component
public class Acervo {
    private final LivroRepository livroRepository;

    @Autowired
    public Acervo(LivroRepository livroRepository) {
        this.livroRepository = livroRepository;
    }

    @PostConstruct
    public void init() {
        if(livroRepository.count() == 0) {
            livroRepository.save(new Livro(01, "OOP: The worst thing to ever happen to programming", "Alexander Danilov", 2024));
            livroRepository.save(new Livro(02, "Java is the worst mistake programmers ever made", "Alexander Danilov", 2024));
            livroRepository.save(new Livro(03, "Computer Networks", "Tanenbaum", 2022));
            livroRepository.save(new Livro(04, "The Rust Programming Language", "Catherine", 2024));
        }
    }

    public List<Livro> getAll() {
        return livroRepository.findAll();
    }

    public List<String> getAutores() {
        return livroRepository.findAll().stream().map(Livro::getAutor).toList();
    }

    public List<String> getTitulos() {
        return livroRepository.findAll().stream().map(Livro::getTitulo).toList();
    }

    public List<Livro> getLivrosAutor(String autor, int ano) {
        return livroRepository.findAll().stream()
                .filter(livro -> livro.getAutor().equals(autor.trim()) && livro.getAno() == ano)
                .toList();
    }

    public List<Livro> getLivrosAno(int ano) {
        return livroRepository.findAll().stream()
                .filter(livro -> livro.getAno() == ano)
                .toList();
    }

    public List<Livro> getLivrosDesatualizados(int ano) {
        return livroRepository.findAll().stream()
                .filter(livro -> livro.getAno() < ano)
                .toList();
    }

    public Livro cadastraLivroNovo(Livro livro) {
        return livroRepository.save(livro);
    }

    public Livro atualizaLivro(Integer codigo, Livro newData) {
        Livro livro = livroRepository.findById(codigo).orElseThrow();
        livroRepository.delete(livro);
        return livroRepository.save(newData);
    }

    public void deletaLivro(Integer codigo) {
        livroRepository.deleteById(codigo);
    }
}
