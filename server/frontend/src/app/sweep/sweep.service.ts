import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { environment as env } from 'src/environments/environment';

@Injectable({
  providedIn: 'root'
})
export class SweepService {

  private url = env.sweep_url;

  constructor(private http: HttpClient) { }

  getSweeps() {
    return this.http.get(this.url);
  }
}
