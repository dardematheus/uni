package com.bcopstein.endpointsdemo1;

import java.util.ArrayList;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class AutorController {
    private final EstatisticaAutor estatisticaAutor;
    private final Acervo acervo;

    @Autowired
    public AutorController(Acervo acervo, EstatisticaAutor estatisticaAutor) {
        this.acervo = acervo;
        this.estatisticaAutor = estatisticaAutor;
    }
    
    @GetMapping("/autores")
    @CrossOrigin(origins = "*")
    public List<String> getAutores() {
        return this.acervo.getAutores();
    }

    @GetMapping("/autores/maispesquisado")
    @CrossOrigin(origins = "*")
    public String maisPesquisado() {
        return this.estatisticaAutor.maisConsultado();
    }

    @GetMapping("/autores/menospesquisado")
    @CrossOrigin(origins = "*")
    public String menosPesquisado() {
        return this.estatisticaAutor.menosConsultado();
    }

    @GetMapping("/livrosautor/{autor}/ano/{ano}")
    @CrossOrigin(origins = "*")
    public List<Livro> getLivrosAutor(@RequestParam(value = "autor") String autor, @PathVariable(value = "ano") int ano) {
        return this.acervo.getLivrosAutor(autor, ano);
    }
}
