import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root'
})
export class SweepService {

  private url = 'http://10.165.76.237:50000/sweep';

  constructor(private http: HttpClient) { }

  getSweeps() {
    return this.http.get(this.url);
  }
}
