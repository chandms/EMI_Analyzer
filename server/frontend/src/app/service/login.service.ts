import { Injectable } from '@angular/core';
import { HttpClient, HttpParams } from '@angular/common/http';
import { environment as env } from 'src/environments/environment';
import jwt_decode from "jwt-decode";
import { FormControl } from '@angular/forms';
import { Login } from '../login/Login';


@Injectable({
  providedIn: 'root'
})
export class LoginService {

  private authentication_url = env.backend_url + '/authentication';

  constructor(private http: HttpClient) { }

  decodeUser(username: string ,password: string){
    let body = {
      'username': username,
      'password': password
    };
    return this.http.post<Login>(this.authentication_url, body);
  }

  
  
}
