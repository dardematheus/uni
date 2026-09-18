package com.bcopstein.endpointsdemo1;

import java.util.HashMap;
import java.util.Map;
import java.util.Set;

import org.springframework.stereotype.Repository;

@Repository 
public class EstatisticaAutor {
    private Map<String, Integer> autores;

    public EstatisticaAutor() {
        this.autores = new HashMap<String, Integer>();
    }

    public void autorPesquisado(String autor) {
        Integer searches = this.autores.get(autor);
        if (searches == null) {
            this.autores.put(autor, 1);
            return;
        }
        this.autores.put(autor, ++searches);
    }

    public String menosConsultado() {
        Map.Entry<String, Integer> lowest = autores.entrySet()
        .stream()
        .min(Map.Entry.comparingByValue())
        .orElse(null);
        return lowest.getKey();
    }

    public String maisConsultado() {
        Map.Entry<String, Integer> highest = autores.entrySet()
        .stream()
        .max(Map.Entry.comparingByValue())
        .orElse(null);
        return highest.getKey();
    }
}
