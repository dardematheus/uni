package com.bcopstein.endpointsdemo1;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class LivroController {
    private final Acervo acervo;

    @Autowired
    public LivroController(Acervo acervo) {
        this.acervo = acervo;
    }

    @GetMapping("/livros")
    @CrossOrigin(origins = "*")
    public List<Livro> getLivros() {
        return this.acervo.getAll();
    }

    @GetMapping("/titulos")
    @CrossOrigin(origins = "*")
    public List<String> getTitulos() {
        return this.acervo.getTitulos();
    }

    @GetMapping("/livrosautor/{autor}/ano/{ano}")
    @CrossOrigin(origins = "*")
    public List<Livro> getLivrosAutor(@RequestParam(value = "autor") String autor, @PathVariable(value = "ano") int ano) {
        return this.acervo.getLivrosAutor(autor, ano);
    }

    @GetMapping("/livrosporano{ano}")
    @CrossOrigin(origins = "*")
    public List<Livro> getLivrosAno(@RequestParam(value = "ano") int ano) {
        return this.acervo.getLivrosAno(ano);
    }

    @GetMapping("/desatualizados/{ano}")
    @CrossOrigin(origins = "*")
    public List<Livro> getLivrosDesatualizados(@RequestParam(value = "ano") int ano) {
        return this.acervo.getLivrosDesatualizados(ano);
    }

    @PostMapping("/livros")
    @CrossOrigin(origins = "*")
    public boolean cadastraLivroNovo(@RequestBody final Livro livro) {
        return this.cadastraLivroNovo(livro);
    }

    @PutMapping("/livros/{codigo}")
    @CrossOrigin(origins = "*")
    public boolean atualizaLivro(@PathVariable int codigo, @RequestBody Livro newData) {
        this.acervo.atualizaLivro(codigo, newData);
        return true;
    }

    @DeleteMapping("/livros/{codigo}")
    @CrossOrigin(origins = "*")
    public boolean deletaLivro(@PathVariable Integer codigo) {
        this.acervo.deletaLivro(codigo);
        return true;
    }
}
